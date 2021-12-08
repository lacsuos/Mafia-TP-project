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
        explicit Connection(std::shared_ptr<Communication> &communication, Base &in_base);

        Connection() = delete;

        ~Connection() = default;

        void start();

        bool isWorking();

        bool isUserWorking() const;

    private:
        void handle_read();

        void handle_write();

        void handle_request();

        void handle_message();

        void disconnect();

        void handle_create_room();

        void handle_join_room();

        void handle_error();

    private:
        io_context &context;

        std::shared_ptr<Communication> communication;
        Base &base;
        std::atomic<bool> is_working;
    };
}
