#include "server.hpp"
#include "connection.hpp"
#include "base.hpp"
#include "game_connection.hpp"

#include <boost/log/trivial.hpp>
#include <boost/shared_ptr.hpp>

namespace bs = boost::system;

namespace net {
    Server::Server() : context_(),
                       endpoint_(address::from_string(std::string(SERVER_IP)), PORT),
                       acceptor_(context_) {}

    Server::~Server() = default;

    void Server::run() {
        context_.post(boost::bind(&Server::StartAccepting, this));
        context_.post(boost::bind(&Server::StartConnection, this));
        context_.post(boost::bind(&Server::CreateRoom, this));

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
        auto connection = std::make_shared<Connection>(context_, base);
        HandleAcception(connection);
    }

    void Server::HandleAcception(std::shared_ptr<Connection> &connection) {
        BOOST_LOG_TRIVIAL(info) << "waiting for acception";
        acceptor_.async_accept(connection->getSocket(), [this, connection](bs::error_code error) {
            if (!error) {
                BOOST_LOG_TRIVIAL(info) << "CONNECTION ACCEPTS";
                connection_mutex_.lock();
                new_connection_.push_back(connection);
                connection_mutex_.unlock();
            } else {
                BOOST_LOG_TRIVIAL(info) << "CONNECTION DOES NOT ACCEPT";
            }
            auto new_user = std::make_shared<Connection>(context_, base);
            context_.post(boost::bind(&Server::HandleAcception, this, new_user));
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


    void Server::CreateRoom() {
        if (base.creating_game.IsEmpty()) {
            context_.post(boost::bind(&Server::CreateRoom, this));
            return;
        }

        auto user = base.creating_game.Pop();
        BOOST_LOG_TRIVIAL(info) << user->getID() << " START NEW GAME";
        auto game = std::make_shared<GameC>(context_, board_db_, user_db_, user);

        game_connection_mutex_.lock();
        new_game_connection_.push_back(gametalker);
        game_connection_mutex_.unlock();
        context_.post(boost::bind(&Server::CreateRoom, this));
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