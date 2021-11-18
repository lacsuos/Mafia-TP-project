#pragma once

#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace net {

    class User {
    public:
        explicit User(io_context &in_context, size_t id_, size_t room_id_) : context(in_context),
                                                                             socket(context),
                                                                             read_buffer(),
                                                                             write_buffer(),
                                                                             in(&read_buffer),
                                                                             out(&write_buffer),
                                                                             is_gaming(false),
                                                                             id(id_),
                                                                             id_room(room_id_) {};

        ~User() = default;

    public:
        io_context &context;
        tcp::socket socket;

        boost::asio::streambuf read_buffer;
        boost::asio::streambuf write_buffer;

        std::istream in;
        std::ostream out;

        boost::atomic<bool> is_gaming;

        size_t id;
        size_t id_room;
        std::string name;
    };
}