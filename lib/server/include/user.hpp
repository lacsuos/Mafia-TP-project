#pragma once

#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace net {

    class User {
    public:
        explicit User(io_context &in_context, size_t id_) : context(in_context),
                                                            socket(context),
                                                            read_buffer(),
                                                            write_buffer(),
                                                            in(&read_buffer),
                                                            out(&write_buffer),
                                                            is_autorised(false),
                                                            id(id_),
                                                            room_id() {};

        ~User() = default;

    public:
        io_context &context;
        tcp::socket socket;

        boost::asio::streambuf read_buffer;
        boost::asio::streambuf write_buffer;

        std::istream in;
        std::ostream out;

        bool is_autorised;
        boost::atomic<bool> is_talking;
        boost::atomic<bool> is_gaming;

        size_t id;
        size_t id_room;
        std::string name;
    };
}