#include "server.hpp"
#include "connection.hpp"

#include <boost/log/trivial.hpp>
#include <boost/shared_ptr.hpp>

namespace bs = boost::system;

namespace net {
    Server::Server() : context_(),
                       endpoint_(address::from_string(std::string(SERVER_IP)), PORT),
                       acceptor_(context_) {}

    Server::~Server() {}

    void Server::StartAccepting() {
        BOOST_LOG_TRIVIAL(info) << "start acceptor on ep = " << endpoint_;
        acceptor_.open(endpoint_.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint_);
        acceptor_.listen();
        auto connection = std::make_shared<Connection>(context_);
        HandleAcception(connection);
    }

    void Server::HandleAcception(std::shared_ptr<Connection> &connection) {
        BOOST_LOG_TRIVIAL(info) << "waiting for acception";
        acceptor_.async_accept(connection->getSocket(), [this, connection](bs::error_code error) {
            if (!error) {
                connection_mutex_.lock();
                BOOST_LOG_TRIVIAL(info) << "user accepted";
                new_connection_.push_back(connection);

                BOOST_LOG_TRIVIAL(info) << "make new user";
                auto new_user = std::make_shared<Connection>(context_);
                BOOST_LOG_TRIVIAL(info) << "before new accepting";
                context_.post(boost::bind(&Server::HandleAcception, this, new_user));
                connection_mutex_.unlock();
            } else {
                BOOST_LOG_TRIVIAL(info) << "user not accepted";
                auto new_user = std::make_shared<Connection>(context_);
                context_.post(boost::bind(&Server::HandleAcception, this, new_user));
            }
        });
    }

    void Server::run() {
        context_.post(boost::bind(&Server::StartAccepting, this));
        context_.post(boost::bind(&Server::StartConnection, this));

        const size_t thread_nom = boost::thread::hardware_concurrency();
        BOOST_LOG_TRIVIAL(info) << "threads avaiable: " << thread_nom;
        StartListen(thread_nom);
        threads_.join_all();
    }

    void Server::StartConnection() {

        connection_mutex_.lock();
        auto connection = std::make_shared<Connection>(context_);
        for (size_t i = 0; i < new_connection_.size(); ++i) {
            BOOST_LOG_TRIVIAL(info) << "worker";
            new_connection_[i]->start();
        }

        connection_mutex_.unlock();
        context_.post(boost::bind(&Server::StartConnection, this));
    }


    void Server::ListenThread() {
        context_.run();
    }

    void Server::StartListen(size_t thread_count) {
        BOOST_LOG_TRIVIAL(info) << "start server";
        for (size_t i = 0; i < thread_count; ++i) {
            threads_.create_thread(boost::bind(&Server::ListenThread, this));
        }
    }
}