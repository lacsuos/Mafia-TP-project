#include "server.hpp"
#include "base_queue.hpp"
#include "messages.hpp"

#include <boost/log/trivial.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace bs = boost::system;

namespace net {
    Server::Server() : context_(),
                       endpoint_(address::from_string(std::string(SERVER_IP)), PORT),
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

    void Server::HandleAcception(std::shared_ptr<Communication> &communication) {
        BOOST_LOG_TRIVIAL(info) << "waiting for acception";
        acceptor_.async_accept(communication->socket, [this, &communication](bs::error_code error) {
            if (!error) {
                auto connection = std::make_shared<Connection>(communication, base);
                BOOST_LOG_TRIVIAL(info) << "CONNECTION ACCEPTS";
                connection_mutex_.lock();
                new_connection_.push_back(connection);
                connection_mutex_.unlock();
            } else {
                BOOST_LOG_TRIVIAL(info) << "CONNECTION DOES NOT ACCEPT";
            }
            auto new_communication = std::make_shared<Communication>(context_);
            context_.post(boost::bind(&Server::HandleAcception, this, new_communication));
        });
    }

    void Server::StartConnection() {

        connection_mutex_.lock();
        for (size_t i = 0; i < new_connection_.size(); ++i) {
            if (!new_connection_[i]->isUserWorking()) {
                BOOST_LOG_TRIVIAL(info) << "DISCONNECTED USER IS REMOVED";
                new_connection_.erase(new_connection_.begin() + i);
            } else if (!new_connection_[i]->isWorking()) {
                BOOST_LOG_TRIVIAL(info) << "START COMMUNICATION";
                new_connection_[i]->start();
            }

        }
        connection_mutex_.unlock();
        context_.post(boost::bind(&Server::StartConnection, this));
    }

    void Server::CleanRemovedRooms() {
        game_connection_mutex_.lock();
        BOOST_LOG_TRIVIAL(info) << " DELETE REMOVED GAME";
        std::erase_if(new_game_connection_,
                      [](const std::shared_ptr<GameConnection> &current) { return current->is_remove.load(); });
        game_connection_mutex_.unlock();
        context_.post(boost::bind(&Server::CleanRemovedRooms, this));
    }


    void Server::CreateRoom() {
        if (base.creating_game.IsEmpty()) {
            context_.post(boost::bind(&Server::CreateRoom, this));
            return;
        }

        auto communication = base.creating_game.Pop();
        BOOST_LOG_TRIVIAL(info) << communication->user->get_id() << " START NEW GAME";
        auto game_connection = std::make_shared<GameConnection>(communication);

        communication->user->set_room((game_connection->get_game()->get_id()));

        game_connection_mutex_.lock();
        new_game_connection_.push_back(game_connection);
        game_connection_mutex_.unlock();
        context_.post(boost::bind(&Server::CreateRoom, this));
    }

    void Server::JoinRoom() {
        if (base.accepting_game.IsEmpty()) {
            context_.post(boost::bind(&Server::JoinRoom, this));
            return;
        }

        auto communication = base.accepting_game.Pop();
        BOOST_LOG_TRIVIAL(info) << communication->user->get_id() << " JOINED GAME";

        auto it = std::find_if(new_game_connection_.begin(), new_game_connection_.end(),
                               [communication](const std::shared_ptr<GameConnection> &current) {
                                   return current->get_game()->get_id() ==
                                          communication->user->get_room();
                               });

        if (it == new_game_connection_.end()) {
            BOOST_LOG_TRIVIAL(info) << communication->user->get_id() << " DOES NOT ACCEPT THE ROOM "
                                    << communication->user->get_room();
            read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
            boost::property_tree::read_json(communication->in, communication->last_msg);
            communication->out << MessageServer::join_room_failed(communication->user->get_room());
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