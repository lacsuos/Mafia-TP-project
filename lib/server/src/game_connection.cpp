#include "game_connection.hpp"
#include "messages.hpp"
#include "PlayRoom.h"


#include <vector>
#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>


using boost::asio::async_read;
using boost::asio::async_write;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace net {

    GameConnection::GameConnection(std::shared_ptr<Communication> &communication)
            : game(GameRoom(communication->user.get_id())),
              is_gaming(false),
              is_remove(false),
              context(communication->context),
              game_room() {

        communication->is_gaming.store(true);
        communication->is_talking.store(false);

        game_mutex.lock();
        communications.push_back(communication);
        game_mutex.unlock();

        read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
        pt::read_json(communication->in, communication->last_msg);

        communication->out << MessageServer::create_room_done(communication->user.get_id());
        async_write(communication->socket, communication->write_buffer,
                    [this, communication](bs::error_code error, size_t
                    len) {
                        if (!error) {
                            handle_read(communication);
                        } else {
                            BOOST_LOG_TRIVIAL(info) << "CREATED ROOM OF USER "
                                                    << communication->user.get_id() << " FAILED";
                            create_room_failed(communication);
                            is_remove.store(true);
                        }
                    });
    }

    void GameConnection::create_room_failed(std::shared_ptr<Communication> communication) {
        BOOST_LOG_TRIVIAL(info) << "creating game failed";
        communication->user.set_room(0);
        read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
        pt::read_json(communication->in, communication->last_msg);
        communication->out << MessageServer::create_room_failed();
        write(communication->socket, communication->write_buffer);
        communication->is_gaming.store(false);
        communication->is_gaming.store(false);
    }

    void GameConnection::handle_error(std::shared_ptr<Communication> communication) {
        communication->out << MessageClient::error();
        boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, communication));
        BOOST_LOG_TRIVIAL(info) << communication->user.get_name() << "'s request is unknown";
    }

    void GameConnection::join_to_game_failed(std::shared_ptr<Communication> communication) {
        read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
        pt::read_json(communication->in, communication->last_msg);
        communication->out << MessageServer::join_room_failed(communication->user.get_room());
        write(communication->socket, communication->write_buffer);
        communication->user.set_room(0);
        communication->is_gaming.store(false);
        communication->is_talking.store(false);
    }

    int GameConnection::join_to_game(std::shared_ptr<Communication> &communication) {
        if (is_gaming || is_remove) {
            join_to_game_failed(communication);
            return -1;
        }

        std::lock_guard<std::mutex> lock(game_mutex);

        if (communications.size() > MAX_USERS) {
            BOOST_LOG_TRIVIAL(info) << "JOIN USER " << communication->user.get_id() << " FAILED";
            join_to_game_failed(communication);
            return -1;
        }

        communication->is_gaming.store(true);
        communication->is_talking.store(false);

        communications.push_back(communication);

        read_until(communication->socket, communication->read_buffer, std::string(MSG_END));
        pt::read_json(communication->in, communication->last_msg);

        communication->out << MessageServer::accept_room_done(communication->user.get_id());
        write(communication->socket, communication->write_buffer);

        boost::asio::post(context, boost::bind(&GameConnection::handle_read, this, communication));
        return 0;
    }

    void GameConnection::handle_read(std::shared_ptr<Communication> communication) {
//        BOOST_LOG_TRIVIAL(info) << "START READING";

        async_read_until(communication->socket, communication->read_buffer, std::string(MSG_END),
                         [this, communication](bs::error_code error, size_t len) {
                             if (!error) {
                                 handle_request(communication);
                             } else {
                                 handle_read(communication);
                             }
                         });
    }

    void GameConnection::handle_write(std::shared_ptr<Communication> communication) {
//        BOOST_LOG_TRIVIAL(info) << "START SENDING";
        async_write(communication->socket, communication->write_buffer,
                    [this, communication](bs::error_code error, size_t len) {
                        if (!error) {
                            handle_read(communication);
                        } else {
                            disconnect(communication);
                        }
                    });
    }

    void GameConnection::handle_admin_request(std::shared_ptr<Communication> communication) {
        BOOST_LOG_TRIVIAL(info) << "GameRoom handleing admin request: ";

        auto command = communication->last_msg.get<std::string>("command");
        std::lock_guard<std::mutex> lock(game_mutex);

        if (command == "start_game") {
            if (communications.size() == MAX_USERS) {
                is_gaming.store(true);

                std::vector<int> ids;
                for (auto &com: communications) {
                    ids.push_back(com->user.get_id());
                }
                BOOST_LOG_TRIVIAL(info) << "GameRoom Created";
                PlayRoom temp(ids);
                game_room = temp;


                std::vector<Player *> players = game_room.GetPlayers();
                for (auto &player: players) {
                    BOOST_LOG_TRIVIAL(info) << player->getGlobalId() << "      "
                                            << player->getRole();
                }

                communication->out << MessageServer::start_game(communication->user.get_id());

            } else {
                communication->out << MessageServer::start_game_failed();
            }
            boost::asio::post(context,
                              boost::bind(&GameConnection::handle_write, this, communication));
            return;
        }
        boost::asio::post(context,
                          boost::bind(&GameConnection::handle_error, this, communication));

    }

    void GameConnection::handle_leave(std::shared_ptr<Communication> communication) {
        BOOST_LOG_TRIVIAL(info) << communication->user.get_name() << " LEAVE ROOM";
        game_mutex.lock();
        for (size_t i = 0; i < communications.size(); ++i) {
            if (communications[i]->user.get_name() == communication->user.get_name()) {
                communications.erase(communications.begin() + i);
                break;
            }
        }
        game_mutex.unlock();

        communication->out << MessageServer::leave_room_done();
        write(communication->socket, communication->write_buffer);

        communication->is_gaming.store(false);
        communication->user.set_room(0);

        std::lock_guard<std::mutex> lock(game_mutex);
        if (communications.empty()) {
            boost::asio::post(context, boost::bind(&GameConnection::game_delete, this));
        }
    }

    void GameConnection::game_delete() {
        is_remove.store(true);
        BOOST_LOG_TRIVIAL(info) << "ROOM " << game.get_id() << " DELETED";
    }


    void GameConnection::handle_request(std::shared_ptr<Communication> communication) {
        if (is_remove) {
            boost::asio::post(context,
                              boost::bind(&GameConnection::handle_leave, this, communication));
            return;
        }

        pt::read_json(communication->in, communication->last_msg);
        std::string command_type = communication->last_msg.get<std::string>("command_type");

        if (command_type == "ping") {
            if (is_gaming) {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_game_status, this,
                                              communication));
            } else {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_ping, this,
                                              communication));
            }
            return;
        }

        if (command_type == "room_admin") {
            if (communication->user.get_id() == game.get_admin_id()) {
                boost::asio::post(context, boost::bind(&GameConnection::handle_admin_request, this,
                                                       communication));
            } else {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_error, this, communication));
            }
            return;
        }

        if (is_gaming) {
            if (command_type != "game") {
                boost::asio::post(context,
                                  boost::bind(&GameConnection::handle_error, this, communication));
            }

//            std::string command = communication->last_msg.get<std::string>("command");
//
//            if (command == "day") {
//                boost::asio::post(context,
//                                  boost::bind(&GameConnection::handle_game_day, this,
//                                              communication));
//            }
//
//            if (command == "vote") {
//                boost::asio::post(context,
//                                  boost::bind(&GameConnection::handle_vote, this,
//                                              communication));
//            }
//
            //            if (command == "`nigth`") {
//                boost::asio::post(context,
//                                  boost::bind(&GameConnection::handle_nigth, this,
//                                              communication));
//
//            } else {
//                boost::asio::post(context,
//                                  boost::bind(&GameConnection::handle_error, this, communication));
//            }
            return;
        }

        //TODO проверку ведущего

    }


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

    void GameConnection::handle_game_status(std::shared_ptr<Communication> communication) {
        std::vector<Player *> players = game_room.GetPlayers();

        int role = 0;
        for (auto &player: players) {
            BOOST_LOG_TRIVIAL(info) << player->getGlobalId() << "      " << player->getRole();
            BOOST_LOG_TRIVIAL(info) << communication->user.get_id();

            if (player->getGlobalId() == communication->user.get_id()) {
                role = player->getRole();
                break;
            }
        }

        //621 1.3 621 2.3

        std::string users_ids;
        std::string users_ips;

        for (auto &i: communications) {
            users_ids += (i->user.get_name());
            users_ids += ";";
            users_ips += (i->user.get_IP());
            users_ips += ";";
        }
        communication->out << MessageServer::connected(users_ids, users_ips, role);
        boost::asio::post(context,
                          boost::bind(&GameConnection::handle_write, this, communication));
    }

    void GameConnection::disconnect(std::shared_ptr<Communication> communication) {
        BOOST_LOG_TRIVIAL(info) << "DISCONNECTED";

        communication->out << MessageClient::disconnect();
        async_write(communication->socket, communication->write_buffer,
                    [this, &communication](bs::error_code error, size_t len) {
                        auto it = std::find(communications.begin(), communications.end(),
                                            communication);
                        if (it != communications.end()) {
                            communications.erase(it);
                        }
                        is_remove.store(true);
                        //
                        communication->socket.close();
                    });
    }

    void GameConnection::handle_ping(std::shared_ptr<Communication> communication) {
        std::string users_ids;
        std::string users_names;

        for (auto &i: communications) {
            users_ids += (std::to_string(i->user.get_id()));
            users_ids += ";";
            users_names += (i->user.get_name());
            users_names += ";";
        }
        communication->out << MessageServer::msg(users_ids, users_names);
        boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, communication));
    }

}
