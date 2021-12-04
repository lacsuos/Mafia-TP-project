#pragma once

#include <string>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <utility>

using boost::asio::ip::tcp;

static size_t id_user_generate = 1;

class User {
public:
    explicit User(const tcp::socket &socket_) : is_user_connecting(false),
                                                is_user_gaming(false),
                                                ip(socket_.remote_endpoint().address().to_string()) {
        name = ip;
        user_id = id_user_generate++;
    };

    User() = delete;

    ~User() = default;

    void set_name(std::string name_) { name = name_; }

    std::string get_name() const { return name; }

    size_t get_id() const { return user_id; }

    void set_room(const size_t &room_id_) { room_id = room_id_; }

    size_t get_room() const { return room_id; }

    std::string get_IP() const { return ip; }

    std::atomic<bool> is_user_connecting;

    std::atomic<bool> is_user_gaming;

private:
    std::string name;
    std::string ip;

    size_t user_id;
    size_t room_id;
};

