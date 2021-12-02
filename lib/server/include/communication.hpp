#pragma once

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace net {

    class Communication {
    public:
        explicit Communication(io_context &in_context) : context(in_context),
                                                         socket(context),
                                                         read_buffer(),
                                                         write_buffer(),
                                                         in(&read_buffer),
                                                         out(&write_buffer) {
            User temp(socket);
            user = &temp;
        };

        Communication() = delete;
        ~Communication() = default;

    public:
        io_context &context;
        tcp::socket socket;

        boost::asio::streambuf read_buffer;
        boost::asio::streambuf write_buffer;

        std::istream in;
        std::ostream out;

        boost::property_tree::ptree last_msg;

        User *user;
    };

}