#include "connection.hpp"
#include "base_queue.hpp"
#include "messages.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>
#include <boost/atomic.hpp>

using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace net {

    Connection::Connection(std::shared_ptr<Communication> &communication, Base &in_base)
            : is_remove(false),
              context(communication->context),
              base(in_base) {
//        communication->user->is_user_connecting.store(false);
//        communication->user->set_ip(communication->socket);
    }

    void Connection::start() {
        communication->is_talking.store(true);
        BOOST_LOG_TRIVIAL(info) << "UserTalker start work with connection";
        boost::asio::post(context, boost::bind(&Connection::handle_read, this));
    }

    void Connection::handle_read() {
        BOOST_LOG_TRIVIAL(info) << "START READING";

        async_read_until(communication->socket, communication->read_buffer, std::string(MSG_END),
                         [this](bs::error_code error, size_t len) {
                             if (!error) {
                                 handle_request();
                             } else {
                                 //todo timer
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

    void Connection::handle_error() {
        BOOST_LOG_TRIVIAL(info) << communication->user.get_name() << "'s request is unknown";

        communication->out << MessageClient::error();
        boost::asio::post(context, boost::bind(&Connection::handle_write, this));
    }

    void Connection::handle_create_room() {
        communication->out << MessageClient::create_room();
        async_write(communication->socket, communication->write_buffer,
                    [this](bs::error_code error, size_t len) {
                        if (!error) {
                            BOOST_LOG_TRIVIAL(info) << communication->user.get_id()
                                                    << " CREATED ROOM";
                            base.creating_game.Push(communication);
                        } else {
                            disconnect();
                        }
                    });
    }

    void Connection::handle_join_room() {

        const pt::ptree &parametrs = communication->last_msg.get_child("parametrs");
        communication->user.set_room(parametrs.get<int>("id"));

        communication->out << MessageClient::join_room(communication->user.get_room());

        async_write(communication->socket, communication->write_buffer,
                    [this](bs::error_code error, size_t len) {
                        if (!error) {
                            BOOST_LOG_TRIVIAL(info) << communication->user.get_id()
                                                    << " JOINED ROOM ID = "
                                                    << communication->user.get_room();
                            base.accepting_game.Push(communication);
                        } else {
                            disconnect();
                        }
                    });
    }

    void Connection::handle_message() {
        communication->out << MessageClient::msg();
        boost::asio::post(context, boost::bind(&Connection::handle_write, this));
    }

    void Connection::disconnect() {
        BOOST_LOG_TRIVIAL(info) << "DISCONNECTED";

        communication->out << MessageClient::disconnect();
        async_write(communication->socket, communication->write_buffer,
                    [this](bs::error_code error, size_t len) {
                        communication->socket.close();
                        is_remove.store(true);
                    });
    }

    void Connection::handle_request() {
        pt::read_json(communication->in, communication->last_msg);
        std::string command_type = communication->last_msg.get<std::string>("command_type");

        if (command_type == "ping") {
            boost::asio::post(context, boost::bind(&Connection::handle_message, this));
            return;
        }

        if (command_type == "disconnect") {
            boost::asio::post(context, boost::bind(&Connection::disconnect, this));
            return;
        }

        if (command_type != "basic") {
            boost::asio::post(context, boost::bind(&Connection::handle_error, this));
            return;
        }

        std::string command = communication->last_msg.get<std::string>("command");

        if (command == "add_room") {
            boost::asio::post(context, boost::bind(&Connection::handle_create_room, this));
        } else if (command == "join_room") {
            boost::asio::post(context, boost::bind(&Connection::handle_join_room, this));
        } else {
            boost::asio::post(context, boost::bind(&Connection::handle_error, this));
        }
    }

    bool Connection::is_working() const {
        return (communication->is_talking.load() || communication->is_gaming.load());
    }
}
