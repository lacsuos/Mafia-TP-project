#include "game_connection.hpp"
#include "messages.hpp"
#include "PlayRoom.h"

#include <vector>
#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>


using boost::asio::async_read;
using boost::asio::async_write;
using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace pt = boost::property_tree;
namespace bs = boost::system;

namespace net {

    GameConnection::GameConnection(std::shared_ptr<Communication> &communication)
            : game(GameRoom(communication->user.get_id())),
              is_gaming(false),
              is_remove(false),
              context(communication->context) {

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
                game_room = std::move(temp);

                const std::vector<std::shared_ptr<Player>> &players = game_room.GetPlayers();
                for (auto player: players) {
                    for (auto i: communications) {
                        i->user.set_role(static_cast<int> (player->getRole()));
                    }
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
        std::lock_guard<std::mutex> lock(game_mutex);
        for (size_t i = 0; i < communications.size(); ++i) {
            if (communications[i]->user.get_name() == communication->user.get_name()) {
                communications.erase(communications.begin() + i);
                break;
            }
        }

        communication->out << MessageServer::leave_room_done();
        write(communication->socket, communication->write_buffer);

        communication->is_gaming.store(false);
        communication->user.set_room(0);

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

            std::string command = communication->last_msg.get<std::string>("command");

            if (communication->user.get_role() == 777) {
                if (command == "day") {
                    boost::asio::post(context,
                                      boost::bind(&GameConnection::handle_game_day, this,
                                                  communication));
                } else if (command == "nigth") {
                    boost::asio::post(context,
                                      boost::bind(&GameConnection::handle_game_nigth, this,
                                                  communication));

                } else {
                    boost::asio::post(context,
                                      boost::bind(&GameConnection::handle_error, this,
                                                  communication));
                }
            } else if (communication->user.get_role() == 2) {
                if (command == "vote") {
                    boost::asio::post(context,
                                      boost::bind(&GameConnection::handle_vote_mafia, this,
                                                  communication));

                } else {
                    boost::asio::post(context,
                                      boost::bind(&GameConnection::handle_error, this,
                                                  communication));
                }
            } else {
                if (command == "vote") {
                    boost::asio::post(context,
                                      boost::bind(&GameConnection::handle_vote, this,
                                                  communication));

                } else {
                    boost::asio::post(context,
                                      boost::bind(&GameConnection::handle_error, this,
                                                  communication));
                }
            }
            return;
        }
    }


    void GameConnection::handle_game_day(std::shared_ptr<Communication> communication) {
        if (static_cast<int>(game_room.day()) == 0) {
            // игра продолжается
            communication->out << MessageServer::day_accepted();
        } else {
            // игра закончена
            communication->out << MessageServer::finish_game();
        }
        boost::asio::post(context,
                          boost::bind(&GameConnection::handle_write, this, communication));
    }

    void GameConnection::handle_vote(std::shared_ptr<Communication> communication) {
        const pt::ptree &parametrs = communication->last_msg.get_child("parametrs");
        auto id = parametrs.get<int>("id");

        std::lock_guard<std::mutex> lock(game_mutex);
        votes.push_back(id);

        if (votes.size() == MAX_USERS) {
            if (static_cast<int>(game_room.evening(votes)) == 0) {
                // игра продолжается
                communication->out << MessageServer::vote_accepted(id);
            } else {
                // игра закончена
                communication->out << MessageServer::finish_game();
            }
        } else {
            communication->out << MessageServer::vote_accepted(id);
        }
        boost::asio::post(context,
                          boost::bind(&GameConnection::handle_write, this, communication));
    }

    void GameConnection::handle_vote_mafia(std::shared_ptr<Communication> communication) {
        const pt::ptree &parametrs = communication->last_msg.get_child("parametrs");
        auto id = parametrs.get<int>("id");

        std::lock_guard<std::mutex> lock(game_mutex);
        votes_mafia.push_back(id);

        communication->out << MessageServer::vote_accepted(id);
        boost::asio::post(context,
                          boost::bind(&GameConnection::handle_write, this, communication));

    }

    void GameConnection::handle_game_nigth(std::shared_ptr<Communication> communication) {
        int killed_id = 0;
        if (votes_mafia.size() == 1) {
            killed_id = game_room.night(votes_mafia);
            communication->out << MessageServer::nigth_accepted(killed_id);
        } else {
            communication->out << MessageClient::error(); // добавить пинг
        }
        boost::asio::post(context,
                          boost::bind(&GameConnection::handle_write, this, communication));
    }

    void GameConnection::handle_game_status(std::shared_ptr<Communication> communication) {
        int role = 0;
        bool is_alive, is_sleep;
        std::string who_is_alive;
        const std::vector<std::shared_ptr<Player>> &players = game_room.GetPlayers();
        for (auto &player: players) {
            who_is_alive += std::to_string(player->getGlobalId());
            who_is_alive += "-";
            who_is_alive += (player->getIsAlive()) ? "ON" : "OFF";
            who_is_alive += ";";
            if (player->getGlobalId() == communication->user.get_id()) {
                role = static_cast<int> (player->getRole());
                is_alive = player->getIsAlive();
                is_sleep = player->getIsSleep();
                break;
            }
        }

        int sts = static_cast<int>(game_room.IsGameOver());
        std::string status;
        switch (sts) {
            case 0:
                status = "GAMING";
                break;
            case 1:
                status = "CITIZEN_WIN";
                break;
            case 2:
                status = "MAFIA_WIN";
                break;
            default:
                break;
        }

        std::string users_ips;

        for (auto &i: communications) {
            users_ips += (i->user.get_IP());
            users_ips += ";";
        }
        communication->out
                << MessageServer::connected(who_is_alive, users_ips, role, is_alive, is_sleep,
                                            status);
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
            users_names += std::to_string(i->user.get_id());
            users_names += ";";
        }
        communication->out << MessageServer::msg(users_ids, users_names);
        boost::asio::post(context, boost::bind(&GameConnection::handle_write, this, communication));
    }

}
