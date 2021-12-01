#pragma once

#include <string>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <utility>

using boost::asio::ip::tcp;

namespace net {
    class User {
    public:
        explicit User(const tcp::socket &socket_, std::string name_) : is_user_connecting(false),
                                                                       is_user_gaming(false),
                                                                       name(std::move(name_)),
                                                                       ip(socket_.remote_endpoint().address().to_string()) {
            user_id = id++;
        };

        User() = delete;

        ~User() = default;

        std::string getIPclient() const { return ip; }

        std::string getName() const { return name; }

        size_t getID() const { return user_id; }

        std::atomic<bool> is_user_connecting;
        std::atomic<bool> is_user_gaming;

    private:
        std::string name;
        std::string ip;

        static size_t id;
        size_t user_id;

    };

    size_t User::id = 1;

}