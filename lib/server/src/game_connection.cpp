#include "game_connection.hpp"
#include "messages.hpp"
#include "PlayRoom.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>


using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace net {

    GameConnection::GameConnection(std::shared_ptr<Communication> &communication)
            : context(communication->context),
              is_gaming(false) {
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

    void GameConnection::handle_error(std::shared_ptr<Communication> &communication) {
        communication->out << Message::error();
        boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, communication));
        BOOST_LOG_TRIVIAL(info) << communication->user->get_name() << "'s request is unknown";
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


        read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
        pt::read_json(communication->in, communication->last_msg);

        communication->out << Message::accept_room_done(communication->user->get_id());
        write(communication->socket, communication->write_buffer);

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

    void GameConnection::handle_admin_request(const std::shared_ptr<Communication> &communication) {
        BOOST_LOG_TRIVIAL(info) << "GameRoom handleing admin request: ";

        std::string command = communication->last_msg.get<std::string>("command_type");

        if (command == "start-game") {
            if (communications.size() > 1) {
                is_gaming.store(true);
                communication->out << Message::start_game(communication->user->get_id());
//                boost::asio::post(context, boost::bind(&GameConnection::Start, this));
            } else {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_error, this, communication));
            }
            boost::asio::post(context,
                              boost::bind(&GameConnection::handle_write, this, communication));
            return;
        }

        boost::asio::post(context, boost::bind(&GameConnection::handle_error, this, communication));
    }

    void GameConnection::handle_request(const std::shared_ptr<Communication> &communication) {
        pt::read_json(communication->in, communication->last_msg);
        std::string command_type = communication->last_msg.get<std::string>("command");

        if (command_type == "message") {
            boost::asio::post(context,
                              boost::bind(&GameConnection::handle_message, this, communication));
            return;
        }

        if (is_gaming) {
            // 1 раз вызывается
            if (command_type == "game") {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_start_game, this,
                                              communication));
            }

            if (command_type == "day") {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_game_day, this,
                                              communication));
            }

            if (command_type == "vote") {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_vote, this,
                                              communication));
            }

            if (command_type == "nigth") {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_nigth, this,
                                              communication));

            } else {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_error, this, communication));
            }
            return;
        }

        //TODO проверку ведущего





        if (command_type == "room-admin") {
            if (communication->user->get_id() == get_game()->get_admin_id()) {
                boost::asio::post(context, boost::bind(&GameConnection::handle_admin_request, this,
                                                       communication));
            } else {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_error, this, communication));
            }
            return;
        }
    }

//    void GameConnection::handle_start_game(const std::shared_ptr<Communication> &communication) {
////        pt::read_json(communication->in, communication->last_msg);
////        std::string command_type = communication->last_msg.get<std::string>("parametrs");
//        std::vector<int> ids;
//        for (auto &com: communications) {
//            ids.push_back(com->user->get_id());
//        }
//
//        PlayRoom play_game_temp(ids);
//        game_room = &play_game_temp;
//
//        std::vector<Player *> players = game_room->getPlayers();
//
//        for (auto &player: players) {
//            if (player->getGlobalId() == communication->user->get_id()) {
//                communication->out << MessageServer::game_start(player->getRole());
//            }
//            //TODO добавить find_if
//        }
//
//        for (auto &comm : communications) {
//            boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, comm));
//            return;
//        }
//
//    }

//    void GameConnection::handle_game_day(const std::shared_ptr<Communication> &communication) {
//        if (!game_room->day()) {
//            // игра продолжается
//        } else {
//            // игра закончена
//        }
//
//        for (auto &comm : communications) {
//            boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, comm));
//            return;
//        }
//    }

//    void GameConnection::handle_vote(const std::shared_ptr<Communication> &communication) {
//        pt::read_json(communication->in, communication->last_msg);
//        int id = communication->last_msg.get<int>("id");
//
//        game_mutex.lock();
//        votes.push_back(id);
//        game_mutex.unlock();
//
//
//        if (votes.size() == MAX_USERS) {
//            if (!game_room->evening(votes)) {
//                int killed_id = game_room->CountingVotes(votes);
//                // игра продолжается
//            } else {
//                // игра закончена
//            }
//        }
//
//        // TODO нужно получить все голоса вначале, а потом уже вызвать метод handle_write
//        for (auto &comm : communications) {
//            boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, comm));
//            return;
//        }
//    }

//    void GameConnection::handle_nigth(const std::shared_ptr<Communication> &communication) {
//        pt::read_json(communication->in, communication->last_msg);
//        int id = communication->last_msg.get<int>("id");
//
//        int killed_id = 0;
//        killed_id = game_room->night(votes)
//
//        // TODO нужно получить все голоса вначале мафии, а потом уже вызвать метод handle_write
//        for (auto &comm: communications) {
//            boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, comm));
//            return;
//        }
//    }

    void GameConnection::handle_message(const std::shared_ptr<Communication> &communication) {
        std::vector<std::vector<std::string>> users_ip;
        users_ip.resize(communications.size());

        for (auto &i: communications) {
            users_ip[0].push_back(i->user->get_name());
            users_ip[1].push_back(i->user->get_IP());
        }
        communication->out << Message::connected(users_ip);
        boost::asio::post(context,
                          boost::bind(&GameConnection::handle_write, this, communication));
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
