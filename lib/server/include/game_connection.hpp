#pragma once

#include "gameroom.hpp"
#include "base.hpp"
#include "user.hpp"

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

constexpr size_t MAX_USERS = 2;

namespace net {

    class GameConnection {
    public:
        explicit GameConnection(io_context &in_context, Base &in_base,
                                std::shared_ptr<User> &user);

        bool isWorking();

        void JoinUserToGame(std::shared_ptr<User> &user);

    private:

        /// Handle completion of a read operation.
        void handle_read(const std::shared_ptr<User>& user);

        /// Handle completion of a write operation.
        void handle_write(const std::shared_ptr<User>& user);

        /// Handle completion of commands.
        void handle_request(const std::shared_ptr<User>& user);

        /// Handle message
        void handle_message(const std::shared_ptr<User>& user);

        /// Handle out
        void disconnect(std::shared_ptr<User>& user);


    private:
        io_context &context;

        std::atomic<bool> is_working{};

        GameRoom* game_;
        std::mutex game_mutex;

        Base &base;
    };
}
