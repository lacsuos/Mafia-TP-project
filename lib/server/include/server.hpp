#pragma once

#include "connection.hpp"
#include "game_connection.hpp"
#include "base.hpp"
#include "communication.hpp"

#include <string_view>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;


constexpr std::string_view SERVER_IP = "127.0.0.1";  // for local game
//constexpr std::string_view SERVER_IP = "0.0.0.0";  // for public game

constexpr uint16_t PORT = 5000;

namespace net {
    class Server {
    public:
        Server();

        Server(Server &other) = delete;

        ~Server();

        void run();

    private:
        void StartListen(size_t thread_count);

        void ListenThread();

        void StartAccepting();

        void CreateRoom();

        void JoinRoom();

        void HandleAcception(std::shared_ptr<Communication> &communication);

        void StartConnection();

        void CleanRemovedRooms();

    private:
        io_context context_;

        tcp::endpoint endpoint_;
        tcp::acceptor acceptor_;

        boost::thread_group threads_;

        std::vector<std::shared_ptr<Connection>> new_connection_;
        std::mutex connection_mutex_;

        std::vector<std::shared_ptr<GameConnection>> new_game_connection_;
        std::mutex game_connection_mutex_;

        Base base;
    };

}