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
                                   std::shared_ptr<User> &user) : context(in_context),
                                                                  socket(context),
                                                                  read_buffer(),
                                                                  write_buffer(),
                                                                  in(&read_buffer),
                                                                  out(&write_buffer),
                                                                  base(in_base) {
        is_working.store(false);
        game_->is_game_connecting.store(true);
        user->is_user_gaming.store(true);

        game_mutex.lock();
        game_->addUser(user);
        game_mutex.unlock();

        read_until(socket, read_buffer, std::string(MSG_END));
        pt::read_json(in, last_msg);

        out << Message::create_room_done(user->getID());
        async_write(socket, write_buffer, [this, user](bs::error_code error, size_t len) {
            if (!error) {
                handle_read(user);
            } else {
                BOOST_LOG_TRIVIAL(info) << "CREATED ROOM OF USER " << user->getID() << " FAILED";
                /// добавить
                game_->is_game_connecting.store(false);
                user->is_user_gaming.store(false);
            }
        });
    }

    void GameConnection::JoinUserToGame(std::shared_ptr<User> &user) {
        if (user->is_user_gaming) {
            BOOST_LOG_TRIVIAL(info) << "JOIN TO ROOM OF USER " << user->getID() << " FAILED";
            return;
        }

        if (game_->getSize() >= MAX_USERS) {
            BOOST_LOG_TRIVIAL(info) << "JOIN USER " << user->getID() << " FAILED";
            return;
        }

        user->is_user_gaming.store(true);

        game_mutex.lock();
        game_->addUser(user);
        game_mutex.unlock();

        boost::asio::post(context, boost::bind(&GameConnection::handle_read, this, user));
    }


    bool GameConnection::isWorking() {
        return is_working.load();
    }


    void GameConnection::handle_read(const std::shared_ptr<User>& user) {
        BOOST_LOG_TRIVIAL(info) << "START READING";

        async_read_until(socket, read_buffer, std::string(MSG_END),
                         [this, &user](bs::error_code error, size_t len) {
                             if (!error) {
                                 handle_request(user);
                             } else {
                                 handle_read(user);
                             }
                         });
    }

    void GameConnection::handle_write(const std::shared_ptr<User>& user) {
        BOOST_LOG_TRIVIAL(info) << "START SENDING";
        async_write(socket, write_buffer, [this, &user](bs::error_code error, size_t len) {
            if (!error) {
                handle_request(user);
            } else {
                disconnect(const_cast<std::shared_ptr<User> &>(user));
            }
        });
    }

    void GameConnection::handle_request(const std::shared_ptr<User>& user) {
        pt::read_json(in, last_msg);
        std::string command_type = last_msg.get<std::string>("command");

        if (command_type == "message") {
            boost::asio::post(context, boost::bind(&GameConnection::handle_message, this, user));
            return;
        }

    }

    void GameConnection::handle_message(const std::shared_ptr<User>& user) {
        std::vector<std::vector<std::string>> users_ip;
        users_ip.resize(game_->getSize());

        std::vector<std::string> temp;
        for (size_t i = 0; i < game_->getSize(); ++i) {
            temp.push_back(game_->getUser(i)->getName());
            for (size_t j = 1; j < game_->getSize(); ++j) {
                temp.push_back(game_->getUser(i)->getIP());
            }
            users_ip.push_back(temp);
            temp.clear();
        }
        out << Message::connected(users_ip);
        boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, user));
    }

    void GameConnection::disconnect(std::shared_ptr<User>& user) {
        BOOST_LOG_TRIVIAL(info) << "DISCONNECTED";

        out << Message::disconnect();
        async_write(socket, write_buffer, [this, &user](bs::error_code error, size_t len) {
            user->is_user_gaming.store(false);
            socket.close();
        });
    }


}