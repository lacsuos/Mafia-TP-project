#include "game_connection.hpp"
#include "messages.hpp"
#include "gameroom.hpp"
#include "base.hpp"
#include "communication.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>


using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace net {

    GameConnection::GameConnection(std::shared_ptr<Communication> &communication)
            : context(communication->context),
              is_working(false) {
        GameRoom temp(communication->user->get_id());
        game = &temp;

        game->is_game_connecting.store(true);
        communication->user->is_user_gaming.store(true);

        game_mutex.lock();

        communications.push_back(communication);
        game_mutex.unlock();

        read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
        pt::read_json(communication->in, communication->last_msg);

        communication->out << Message::create_room_done(communication->user->get_id());
        async_write(communication->socket, communication->write_buffer,
                    [this, communication](bs::error_code error, size_t
                    len) {
                        if (!error) {
                            handle_read(communication);
                        } else {
                            BOOST_LOG_TRIVIAL(info) << "CREATED ROOM OF USER "
                                                    << communication->user->get_id() << " FAILED";
                            game->is_game_connecting.store(false);
                            communication->user->is_user_gaming.store(false);
                        }
                    });
    }

    void GameConnection::join_to_game(std::shared_ptr<Communication> &communication) {
        if (communication->user->is_user_gaming) {
            BOOST_LOG_TRIVIAL(info) << "JOIN TO ROOM OF USER " << communication->user->get_id()
                                    << " FAILED";
            return;
        }

        if (communications.size() > MAX_USERS) {
            BOOST_LOG_TRIVIAL(info) << "JOIN USER " << communication->user->get_id() << " FAILED";
            return;
        }

        communication->user->is_user_gaming.store(true);

        game_mutex.lock();
        communications.push_back(communication);
        game_mutex.unlock();

        boost::asio::post(context, boost::bind(&GameConnection::handle_read, this, communication));
    }

    void GameConnection::handle_read(const std::shared_ptr<Communication> &communication) {
        BOOST_LOG_TRIVIAL(info) << "START READING";

        async_read_until(communication->socket, communication->read_buffer, std::string(MSG_END),
                         [this, &communication](bs::error_code error, size_t len) {
                             if (!error) {
                                 handle_request(communication);
                             } else {
                                 handle_read(communication);
                             }
                         });
    }

    void GameConnection::handle_write(const std::shared_ptr<Communication> &communication) {
        BOOST_LOG_TRIVIAL(info) << "START SENDING";
        async_write(communication->socket, communication->write_buffer,
                    [this, &communication](bs::error_code error, size_t len) {
                        if (!error) {
                            handle_request(communication);
                        } else {
                            disconnect(const_cast<std::shared_ptr<Communication> &>(communication));
                        }
                    });
    }

    void GameConnection::handle_request(const std::shared_ptr<Communication> &communication) {
        pt::read_json(communication->in, communication->last_msg);
        std::string command_type = communication->last_msg.get<std::string>("command");

        if (command_type == "message") {
            boost::asio::post(context,
                              boost::bind(&GameConnection::handle_message, this, communication));
            return;
        }

    }

    void GameConnection::handle_message(const std::shared_ptr<Communication> &communication) {
        std::vector<std::vector<std::string>> users_ip;
        users_ip.resize(communications.size());

        for (auto &i: communications) {
            users_ip[0].push_back(i->user->get_name());
            users_ip[1].push_back(i->user->get_IP());
        }
        communication->out << Message::connected(users_ip);
        boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, communication));
    }

    void GameConnection::disconnect(std::shared_ptr<Communication> &communication) {
        BOOST_LOG_TRIVIAL(info) << "DISCONNECTED";

        communication->out << Message::disconnect();
        async_write(communication->socket, communication->write_buffer,
                    [this, &communication](bs::error_code error, size_t len) {
                        auto it = std::find(communications.begin(), communications.end(),
                                            communication);
                        if (it != communications.end()) {
                            communications.erase(it);
                        }
                        communication->user->is_user_gaming.store(false);
                        communication->socket.close();
                    });
    }

}
