#pragma once

#include "gameroom.hpp"
#include "base.hpp"
#include "communication.hpp"

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

constexpr size_t MAX_USERS = 4;

namespace net {

    class GameConnection {
    public:
        explicit GameConnection(std::shared_ptr<Communication> &communication);

        void join_to_game(std::shared_ptr<Communication> &communication);

        const GameRoom *get_game() { return game; }

    private:

        /// Handle completion of a read operation.
        void handle_read(const std::shared_ptr<Communication> &communication);

        /// Handle completion of a write operation.
        void handle_write(const std::shared_ptr<Communication> &communication);

        /// Handle completion of commands.
        void handle_request(const std::shared_ptr<Communication> &communication);

        /// Handle message
        void handle_message(const std::shared_ptr<Communication> &communication);

        /// Handle out
        void disconnect(std::shared_ptr<Communication> &communication);

        void handle_start_game(const std::shared_ptr<Communication> &communication);

        void handle_error(std::shared_ptr<Communication> &communication);

        void handle_admin_request(const std::shared_ptr<Communication> &communication);


    private:
        io_context &context;

        std::atomic<bool> is_gaming{};
        std::vector<std::shared_ptr<Communication>> communications;

        GameRoom *game;
        std::mutex game_mutex;
    };
}
