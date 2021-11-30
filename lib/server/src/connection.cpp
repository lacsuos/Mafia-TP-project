#include "connection.hpp"
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

    Connection::Connection(io_context &in_context, Base& in_base) : context(in_context),
                                                     socket(context),
                                                     read_buffer(),
                                                     write_buffer(),
                                                     in(&read_buffer),
                                                     out(&write_buffer),
                                                     base(in_base) {
        is_working.store(false);
        user_->is_gaming.store(false);
    }

    void Connection::start() {
        is_working.store(true);
        user_->is_gaming.store(true);
        boost::asio::post(context, boost::bind(&Connection::handle_read, this));
    }

    bool Connection::isWorking() {
        return is_working.load();
    }

    bool Connection::isUserWorking() const {
        return user_->is_gaming.load();
    }

    void Connection::handle_read() {
        BOOST_LOG_TRIVIAL(info) << "START READING";

        async_read_until(socket, read_buffer, std::string(MSG_END),
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
        async_write(socket, write_buffer, [this](bs::error_code error, size_t len) {
            if (!error) {
                handle_read();
            } else {
                disconnect();
            }
        });
    }

    void Connection::handle_request() {
        pt::read_json(in, last_msg);
        std::string command_type = last_msg.get<std::string>("command");

        if (command_type == "message") {
            boost::asio::post(context, boost::bind(&Connection::handle_message, this));
            return;
        }

        if (command_type == "disconnect") {
            boost::asio::post(context, boost::bind(&Connection::disconnect, this));
            return;
        }

        if (command_type == "disconnect") {
            boost::asio::post(context, boost::bind(&Connection::disconnect, this));
            return;
        }


    }

    void Connection::handle_message() {
        out << Message::msg();
        boost::asio::post(context, boost::bind(&Connection::handle_write, this));
    }

    void Connection::disconnect() {
        BOOST_LOG_TRIVIAL(info) << "DISCONNECTED";

        out << Message::disconnect();
        async_write(socket, write_buffer, [this](bs::error_code error, size_t len) {
            user_->is_gaming.store(false);
            socket.close();
        });
    }
}
