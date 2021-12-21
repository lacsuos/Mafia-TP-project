#pragma once

#include"user.hpp"

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/property_tree/ptree.hpp>

namespace net {

    class Communication {
    public:
        explicit Communication(boost::asio::io_context &in_context) : context(in_context),
                                                         socket(context),
                                                         read_buffer(),
                                                         write_buffer(),
                                                         in(&read_buffer),
                                                         out(&write_buffer),
                                                         is_gaming(false),
                                                         is_talking(false),
                                                         user(User()) {};

        Communication() = delete;

        ~Communication() = default;

    public:
        boost::asio::io_context &context;
        boost::asio::ip::tcp::socket socket;

        boost::asio::streambuf read_buffer;
        boost::asio::streambuf write_buffer;

        std::istream in;
        std::ostream out;

        boost::property_tree::ptree last_msg;

        boost::atomic<bool> is_gaming;
        boost::atomic<bool> is_talking;

        User user;
    };

}