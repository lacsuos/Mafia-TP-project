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
        BOOST_LOG_TRIVIAL(info) << "START ACCEPT ON " << endpoint_;
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
                BOOST_LOG_TRIVIAL(info) << "CONNECTION ACCEPTS";
                connection_mutex_.lock();
                new_connection_.push_back(connection);
                connection_mutex_.unlock();
                auto new_user = std::make_shared<Connection>(context_);
                context_.post(boost::bind(&Server::HandleAcception, this, new_user));
            } else {
                BOOST_LOG_TRIVIAL(info) << "CONNECTION DOES NOT ACCEPT";
                auto new_user = std::make_shared<Connection>(context_);
                context_.post(boost::bind(&Server::HandleAcception, this, new_user));
            }
        });
    }

    void Server::run() {
        context_.post(boost::bind(&Server::StartAccepting, this));
        context_.post(boost::bind(&Server::StartConnection, this));
        const size_t thread_nom = boost::thread::hardware_concurrency();
        BOOST_LOG_TRIVIAL(info) << "AVAILABLE THREADS ARE  " << thread_nom;
        StartListen(thread_nom);
        threads_.join_all();
    }

    void Server::StartConnection() {

        connection_mutex_.lock();
        auto connection = std::make_shared<Connection>(context_);
        for (size_t i = 0; i < new_connection_.size(); ++i) {
            if (!new_connection_[i]->isWorking()){
                BOOST_LOG_TRIVIAL(info) << "START COMMUNICATION";
                new_connection_[i]->start();
            }
        }
        connection_mutex_.unlock();
        context_.post(boost::bind(&Server::StartConnection, this));
    }


    void Server::ListenThread() {
        context_.run();
    }

    void Server::StartListen(size_t thread_count) {
        BOOST_LOG_TRIVIAL(info) << "START SERVER";
        for (size_t i = 0; i < thread_count; ++i) {
            threads_.create_thread(boost::bind(&Server::ListenThread, this));
        }
    }
}