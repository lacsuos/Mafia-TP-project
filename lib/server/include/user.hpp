#pragma once

#include <string>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

namespace net {
    class User {
    public:
        std::atomic<bool> is_gaming;

        std::string getIPclient() { return ip; }

    private:

        tcp::socket socket;
        std::string name;
        size_t room_id;
        std::string ip = socket.remote_endpoint().address().to_string();

        static size_t id;
    };
}