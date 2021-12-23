#pragma once

#include "connection.hpp"
#include "game_connection.hpp"
#include "base.hpp"
#include "communication.hpp"

#include <string_view>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

constexpr uint16_t PORT = 5000;

namespace net {
    class Server {
    public:
        explicit Server(const std::string_view &SERVER_IP);

        Server() = delete;

        ~Server();

        void run();

    private:
        void StartListen(size_t thread_count);

        void ListenThread();

        void StartAccepting();

        void CreateRoom();

        void JoinRoom();

        void HandleAcception(std::shared_ptr<Communication> communication);

        void StartConnection();

        void CleanRemovedRooms();

        void AcceptingFailed();

        void AcceptionDone(std::shared_ptr<Communication> communication);

    private:
        boost::asio::io_context context_;

        boost::asio::ip::tcp::endpoint endpoint_;
        boost::asio::ip::tcp::acceptor acceptor_;

        boost::thread_group threads_;

        std::vector<std::shared_ptr<Connection>> new_connection_;
        std::mutex connection_mutex_;

        std::vector<std::shared_ptr<GameConnection>> new_game_connection_;
        std::mutex game_connection_mutex_;

        Base base;
    };

}