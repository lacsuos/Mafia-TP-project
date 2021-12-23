#include "server.hpp"
#include "base_queue.hpp"
#include "messages.hpp"

#include <vector>
#include <algorithm>

#include <boost/log/trivial.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace bs = boost::system;

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;

namespace net {
    Server::Server(const std::string_view &SERVER_IP) : context_(),
                                                        endpoint_(address::from_string(
                                                                std::string(SERVER_IP)), PORT),
                                                        acceptor_(context_) {}

    Server::~Server() = default;

    void Server::run() {
        context_.post(boost::bind(&Server::StartAccepting, this));
        context_.post(boost::bind(&Server::StartConnection, this));
        context_.post(boost::bind(&Server::CleanRemovedRooms, this));
        context_.post(boost::bind(&Server::CreateRoom, this));
        context_.post(boost::bind(&Server::JoinRoom, this));

        const size_t thread_nom = boost::thread::hardware_concurrency();
        BOOST_LOG_TRIVIAL(info) << "AVAILABLE THREADS ARE  " << thread_nom;
        StartListen(thread_nom);
        threads_.join_all();
    }

    void Server::StartAccepting() {
        BOOST_LOG_TRIVIAL(info) << "START ACCEPT ON " << endpoint_;
        acceptor_.open(endpoint_.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint_);
        acceptor_.listen();
        auto communication = std::make_shared<Communication>(context_);
        HandleAcception(communication);
    }

    void Server::HandleAcception(std::shared_ptr<Communication> communication) {
        BOOST_LOG_TRIVIAL(info) << "waiting for acception";
        acceptor_.async_accept(communication->socket, [this, communication](bs::error_code error) {
            if (!error) {
                AcceptionDone(communication);
            } else {
                AcceptingFailed();
            }
        });
    }

    void Server::AcceptionDone(std::shared_ptr<Communication> communication) {
        auto connection = std::make_shared<Connection>(communication, base);
        BOOST_LOG_TRIVIAL(info) << "CONNECTION ACCEPTS";
        std::lock_guard<std::mutex> lock(connection_mutex_);
        new_connection_.push_back(connection);
        auto new_communication = std::make_shared<Communication>(context_);
        context_.post(boost::bind(&Server::HandleAcception, this, new_communication));
    }

    void Server::AcceptingFailed() {
        BOOST_LOG_TRIVIAL(info) << "CONNECTION DOES NOT ACCEPT";
        auto new_communication = std::make_shared<Communication>(context_);
        context_.post(boost::bind(&Server::HandleAcception, this, new_communication));
    }


    void Server::StartConnection() {

        std::lock_guard<std::mutex> lock(connection_mutex_);
        for (size_t i = 0; i < new_connection_.size(); ++i) {
            if (new_connection_[i]->is_remove.load()) {
                BOOST_LOG_TRIVIAL(info) << "DISCONNECTED USER IS REMOVED";
                new_connection_.erase(new_connection_.begin() + i);
            } else if (!new_connection_[i]->is_working()) {
                BOOST_LOG_TRIVIAL(info) << "START COMMUNICATION";
                new_connection_[i]->start();
            }

        }
        context_.post(boost::bind(&Server::StartConnection, this));
    }

    void Server::CleanRemovedRooms() {
        if (new_game_connection_.empty()) {
            context_.post(boost::bind(&Server::CleanRemovedRooms, this));
            return;
        }

        std::lock_guard<std::mutex> lock(game_connection_mutex_);
        std::erase_if(new_game_connection_,
                      [](const std::shared_ptr<GameConnection> &current) {
                          if (current->is_remove.load()) {
                              BOOST_LOG_TRIVIAL(info) << "DELETE REMOVED GAME";
                          }
                          return current->is_remove.load();
                      });
        context_.post(boost::bind(&Server::CleanRemovedRooms, this));
    }


    void Server::CreateRoom() {
        if (base.creating_game.IsEmpty()) {
            context_.post(boost::bind(&Server::CreateRoom, this));
            return;
        }

        auto communication = base.creating_game.Pop();
        BOOST_LOG_TRIVIAL(info) << communication->user.get_id() << " START NEW GAME";
        auto game_connection = std::make_shared<GameConnection>(communication);

        communication->user.set_room((game_connection->game.get_id()));

        std::lock_guard<std::mutex> lock(game_connection_mutex_);

        new_game_connection_.push_back(game_connection);
        context_.post(boost::bind(&Server::CreateRoom, this));
    }

    void Server::JoinRoom() {
        if (base.accepting_game.IsEmpty()) {
            context_.post(boost::bind(&Server::JoinRoom, this));
            return;
        }

        std::lock_guard<std::mutex> lock(game_connection_mutex_);

        auto communication = base.accepting_game.Pop();
        BOOST_LOG_TRIVIAL(info) << communication->user.get_id() << " JOINED GAME";

        auto it = std::find_if(new_game_connection_.begin(), new_game_connection_.end(),
                               [communication](const std::shared_ptr<GameConnection> &current) {
                                   return current->game.get_id() ==
                                          communication->user.get_room();
                               });

        if (it == new_game_connection_.end()) {
            BOOST_LOG_TRIVIAL(info) << communication->user.get_id() << " DOES NOT ACCEPT THE ROOM "
                                    << communication->user.get_room();
            read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
            boost::property_tree::read_json(communication->in, communication->last_msg);
            communication->out << MessageServer::join_room_failed(communication->user.get_room());
            write(communication->socket, communication->write_buffer);
            context_.post(boost::bind(&Server::JoinRoom, this));
            return;
        }

        it->get()->join_to_game(communication);

        context_.post(boost::bind(&Server::JoinRoom, this));
    }

    void Server::StartListen(size_t thread_count) {
        BOOST_LOG_TRIVIAL(info) << "START SERVER";
        for (size_t i = 0; i < thread_count; ++i) {
            threads_.create_thread(boost::bind(&Server::ListenThread, this));
        }
    }

    void Server::ListenThread() {
        context_.run();
    }
}