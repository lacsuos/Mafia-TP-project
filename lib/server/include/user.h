#pragma once

#include <string>
using boost::asio::ip::tcp;

namespace net {
    class User {
    public:
        bool Connection::isWorking() { return is_working.load(); }

    private:
        tcp::socket socket;

        std::string name;
        size_t room_id;
        std::string ip;
        static size_t id;

        std::atomic<bool> is_gaming;
    };
}