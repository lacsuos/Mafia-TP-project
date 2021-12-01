#include "game_connection.hpp"
#include "messages.hpp"
#include "user.hpp"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>


using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace net {

    GameConnection::GameConnection(io_context &in_context, Base &in_base,
                                   const std::shared_ptr<User> &user) : context(in_context),
                                                                        socket(context),
                                                                        read_buffer(),
                                                                        write_buffer(),
                                                                        in(&read_buffer),
                                                                        out(&write_buffer),
                                                                        base(in_base) {
        is_working.store(false);
        game_->is_game_connecting.store(false);

        game_->addUser(user);
    }
}