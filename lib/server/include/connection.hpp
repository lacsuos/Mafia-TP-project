#pragma once

#include "user.hpp"
#include "base.hpp"
#include "communication.hpp"
#include "base_queue.hpp"

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace net {

    class Connection {
    public:
        /// Construct a connection with the given io_context.
        explicit Connection(std::shared_ptr<Communication> &communication, Base &in_base);

        Connection() = delete;

        ~Connection() = default;

        /// Start the first asynchronous operation for the connection.
        void start();

        bool isWorking();

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

        void handle_create_room();

        void handle_join_room();

    private:
        io_context &context;

        std::shared_ptr<Communication> communication;
        Base &base;
        std::atomic<bool> is_working;
    };
}
