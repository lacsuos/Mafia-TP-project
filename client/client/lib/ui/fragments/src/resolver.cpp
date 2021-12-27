#include "resolver.h"
#include "client.h"

#include <algorithm>
#include <QDebug>
#include <QApplication>
#include <string>
#include <sstream>
#include <vector>

using namespace screens;

constexpr int MAX_PLAYERS = 4;

static std::vector<std::string> split(const std::string &s, char delimetr = ';') {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delimetr)) {
        elems.push_back(item);
    }
    return elems;
}

Resolver::Resolver() : is_admin(false),
                       is_started(false) {}

void Resolver::parse_answer(pt::ptree const &answer) {
    auto command_type = answer.get<std::string>("command_type");

    if (command_type == "ping") {
        return;
    }

    if (command_type == "error") {
        return;
    }

    if (command_type == "disconnect") {
//        disconnect(answer);
        emit serverDisconnected();
        return;
    }

    if (command_type == "basic") {
        base_answer(answer);
        return;
    }

    if (command_type == "basic_room") {
        base_room_answer(answer);
        return;
    }

}

void Resolver::base_answer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "create_room") {
        is_admin = true;
        create_room_answer(answer);
        return;
    }

    if (command == "join_room") {
        join_room_answer(answer);
        return;
    }

    if (command == "pre_game") {
        pregame_answer(answer);
        return;
    }

}

void Resolver::create_room_answer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto status = parametrs.get<std::string>("status");
    if (is_admin) {
        if (status == "in_process") {
            return;
        }

        if (status == "done") {
//
            qDebug() << "Created room recieved";
            /*emit */created();
            is_started = true;
            return;
        }

        if (status == "fail") {
            // error
            emit netError();
        }
    }
}

void Resolver::join_room_answer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto status = parametrs.get<std::string>("status");

    if (status == "in_process") {
        return;
    }

    if (status == "done") {
//
        emit joined();
        return;
    }

    if (status == "fail") {
        emit netError();
    }
}

void Resolver::check_players(const std::vector<resolver::Player> &new_players) {
    for (size_t i = 0; i < players.size(); ++i) {
        auto cit = players.cbegin() + i;
        auto res = std::find_if(new_players.begin(), new_players.end(),
                                [cit](const resolver::Player &current) {
                                    return current.id == cit->id;
                                });
        if (res == new_players.end()) {
//            emit DeletePlayer(cit->position);
            players.erase(cit);
        }
    }

    for (auto &it: new_players) {
        auto res = std::find_if(players.begin(), players.end(),
                                [it](const resolver::Player &current) {
                                    return current.id == it.id;
                                });
        if (res == players.end()) {
            players.push_back(it);
//            emit DrawPlayer();
        }
    }
}

void Resolver::pregame_answer(const boost::property_tree::ptree &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto ids = parametrs.get<std::string>("ids");
    auto names = parametrs.get<std::string>("names");

    std::vector<std::string> ids_array = split(ids);
    std::vector<std::string> names_array = split(names);
    auto id = parametrs.get<int>("id");

    std::vector<resolver::Player> new_players;
    for (size_t i = 0; i < ids_array.size(); ++i) {
        resolver::Player player;
        player.id = std::stoi(ids_array[i]);
        player.name = names_array[i];
        if (player.id == id) {
            player.is_me = true;
        } else {
            player.is_me = false;
        }
        new_players.push_back(player);
    }
    check_players(new_players);
}

void Resolver::base_room_answer(const boost::property_tree::ptree &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "leave") {
        game_leave_answer(answer);
        return;
    }

    if (command == "finish_game") {
        game_finish_answer(answer);
        return;
    }

    if (command == "day") {
        game_day_answer(answer);
        return;
    }

    if (command == "nigth") {
        game_nigth_answer(answer);
        return;
    }

    if (command == "vote") {
        game_vote_answer(answer);
        return;
    }

    if (command == "game_room") {
        if (is_started) {
            game_ping_answer(answer);
            return;
        }
    }
}

void Resolver::game_vote_answer(const boost::property_tree::ptree &answer) {

}

void Resolver::game_leave_answer(const boost::property_tree::ptree &answer) {

}

void Resolver::game_finish_answer(const boost::property_tree::ptree &answer) {

}

void Resolver::game_day_answer(const boost::property_tree::ptree &answer) {

}

void Resolver::game_nigth_answer(const boost::property_tree::ptree &answer) {

}

void Resolver::game_ping_answer(const boost::property_tree::ptree &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto ids = parametrs.get<std::string>("ids");
    auto ips = parametrs.get<std::string>("ips");
    auto id = parametrs.get<int>("id");
    auto role = parametrs.get<int>("role");
    auto alive = parametrs.get<std::string>("status_is_alive");
    auto sleep = parametrs.get<std::string>("status_is_sleep");
    auto status = parametrs.get<std::string>("status");

    if (status == "GAMING") {
        std::vector<std::string> ids_array = split(ids);
        std::vector<std::string> ips_array = split(ips);

        std::vector<resolver::Player> new_players;
        for (size_t i = 0; i < ids_array.size(); ++i) {
            resolver::Player player;
            std::vector<std::string> temp = split(ids_array[i],'-');
            player.id = std::stoi(temp[0]);
            player.is_live = (temp[1] == "ON" ? true : false);
            player.ip = ips_array[i];
            if (player.id == id) {
                player.is_me = true;
                player.role = role;
                player.is_live = (alive == "ON" ? true : false);
                player.is_sleep = (sleep == "ON" ? true : false);
            } else {
                player.is_me = false;
            }
            new_players.push_back(player);
        }
        check_players(new_players);

    } else if (status == "CITIZEN_WIN") {
        if (role == 777) {
//            emit citizenWin;
            return;
        }
        if (role == 2) {
//            emit loseGame;
        } else if (role == 1) {
//            emit winGame;
        }
    } else if (status == "MAFIA_WIN") {
        if (role == 777) {
//            emit mafiaWin;
            return;
        }
        if (role == 2) {
//            emit winGame;
        } else if (role == 1) {
//            emit loseGame;
        }
    }
}

void Resolver::Run() {
    while (Client->isConnected()) {
        auto m = Client->getLastMsg();
//        std::cout << m;
        std::stringstream msg(m);
        pt::ptree json_data;
        pt::read_json(msg, json_data);
        parse_answer(json_data);
    }
}

#include "moc_resolver.cpp" // !!!
