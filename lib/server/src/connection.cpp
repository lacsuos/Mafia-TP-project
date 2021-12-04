#include "connection.hpp"
#include "user.hpp"
#include "base.hpp"
#include "base_queue.hpp"
#include "communication.hpp"
#include "messages.hpp"

#include <boost/asio/yield.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>

using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace net {

    Connection::Connection(std::shared_ptr<Communication> &communication, Base &in_base)
            : context(communication->context),
              base(in_base),
              is_working(false) {
        communication->user->is_user_connecting.store(false);
    }

    void Connection::start() {
        is_working.store(true);
        communication->user->is_user_connecting.store(true);
        boost::asio::post(context, boost::bind(&Connection::handle_read, this));
    }

    bool Connection::isWorking() {
        return is_working.load();
    }

    bool Connection::isUserWorking() const {
        return communication->user->is_user_connecting.load();
    }

    void Connection::handle_read() {
        BOOST_LOG_TRIVIAL(info) << "START READING";

        async_read_until(communication->socket, communication->read_buffer, std::string(MSG_END),
                         [this](bs::error_code error, size_t len) {
                             if (!error) {
                                 handle_request();
                             } else {
                                 handle_read();
                             }
                         });
    }

    void Connection::handle_write() {
        BOOST_LOG_TRIVIAL(info) << "START SENDING";
        async_write(communication->socket, communication->write_buffer,
                    [this](bs::error_code error, size_t len) {
                        if (!error) {
                            handle_read();
                        } else {
                            disconnect();
                        }
                    });
    }

    void Connection::handle_request() {
        pt::read_json(communication->in, communication->last_msg);
        std::string command_type = communication->last_msg.get<std::string>("command");

        if (command_type == "message") {
            boost::asio::post(context, boost::bind(&Connection::handle_message, this));
            return;
        }

        if (command_type == "disconnect") {
            boost::asio::post(context, boost::bind(&Connection::disconnect, this));
            return;
        }

        if (command_type == "add_room") {
            boost::asio::post(context, boost::bind(&Connection::handle_create_room, this));
        }
        if (command_type == "join_room") {
            boost::asio::post(context, boost::bind(&Connection::handle_join_room, this));
        }
    }

    void Connection::handle_create_room() {
        communication->out << Message::create_room();
        async_write(communication->socket, communication->write_buffer,
                    [this](bs::error_code error, size_t len) {
                        if (!error) {
                            BOOST_LOG_TRIVIAL(info) << communication->user->get_id()
                                                    << " CREATED ROOM";
                            base.creating_game.Push(communication);
                        } else {
                            disconnect();
                        }
                    });
    }

    void Connection::handle_join_room() {

        const pt::ptree &parametrs = communication->last_msg.get_child("parametrs");
        communication->user->set_room(parametrs.get<size_t>("id"));

        communication->out << Message::join_room(communication->user->get_room());

        async_write(communication->socket, communication->write_buffer,
                    [this](bs::error_code error, size_t len) {
                        if (!error) {
                            BOOST_LOG_TRIVIAL(info) << communication->user->get_id()
                                                    << " JOINED ROOM ID = "
                                                    << communication->user->get_room();
                            base.accepting_game.Push(communication);
                        } else {
                            disconnect();
                        }
                    });
    }

    void Connection::handle_message() {
        communication->out << Message::msg();
        boost::asio::post(context, boost::bind(&Connection::handle_write, this));
    }

    void Connection::disconnect() {
        BOOST_LOG_TRIVIAL(info) << "DISCONNECTED";

        communication->out << Message::disconnect();
        async_write(communication->socket, communication->write_buffer,
                    [this](bs::error_code error, size_t len) {
                        communication->user->is_user_connecting.store(false);
                        communication->socket.close();
                    });
    }
}
