#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;


// constexpr std::string_view SERVER_IP = "127.0.1.0";  // for local game
constexpr std::string_view SERVER_IP = "0.0.0.0";  // for public game

constexpr uint16_t PORT = 5000;

namespace net {
    class Server {
    public:
        Server();
        Server(Server &other) = delete;
        ~Server();
        void Start();

    private:
        void StartListen(size_t thread_count);
        void ListenThread();
        void StartAccepting();

        void CreateRooms();
        void JoinPlayers();

        void HandleAcception(std::shared_ptr<User> &user);
        void AcceptionDone(std::shared_ptr<User> user);
        void AcceptionFailed();

    private:
        io_context context_;

        std::vector<std::shared_ptr<GameTalker>> gametalkers_;
        std::mutex gametalkers_mutex_;

        tcp::endpoint endpoint_;
        tcp::acceptor acceptor_;

    };

}  // namespace network

