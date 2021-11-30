#pragma once
#include "user.hpp"

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace net {

    class Connection {
    public:
        /// Construct a connection with the given io_context.
        explicit Connection(io_context &in_context);

        /// Start the first asynchronous operation for the connection.
        void start();

        bool isWorking();

        tcp::socket& getSocket() { return socket; }

        bool isUserWorking() const;
    private:
        /// Handle completion of a read operation.
        void handle_read();

        /// Handle completion of a write operation.
        void handle_write();

        /// Handle completion of commands.
        void handle_request();

        /// Handle message
        void handle_message();

        /// Handle out
        void disconnect();

    private:
        io_context &context;
        tcp::socket socket;

        boost::asio::streambuf read_buffer;
        boost::asio::streambuf write_buffer;

        std::istream in;
        std::ostream out;
        std::atomic<bool> is_working;
        boost::property_tree::ptree last_msg;

        std::shared_ptr<User> user_;
    };
}
