#pragma once

#include <string>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <utility>

using boost::asio::ip::tcp;

static int id_user_generate = 1;

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

    void set_name(const std::string &name_) { name = name_; }

    std::string get_name() const { return name; }

    int get_id() const { return user_id; }

    void set_room(const int &room_id_) { room_id = room_id_; }

    int get_room() const { return room_id; }

    std::string get_IP() const { return ip; }

    std::atomic<bool> is_user_connecting;

    std::atomic<bool> is_user_gaming;

private:
    std::string name;
    std::string ip;

    int user_id;
    int room_id;
};

