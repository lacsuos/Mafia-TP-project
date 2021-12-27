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

Resolver::Resolver() : isFirst(true),
                       isAdmin(false),
                       isStarted(false) {}

void Resolver::parse_answer(pt::ptree const &answer) {
    auto command_type = answer.get<std::string>("command_type");

    if (command_type == "ping") {
        return;
    }

    if (command_type == "error") {
        return;
    }

    if (command_type == "disconnect") {
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
        isAdmin = true;
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
    if (isAdmin) {
        if (status == "in_process") {
            return;
        }

        if (status == "done") {
            auto id = parametrs.get<int>("id");
            qDebug() << "Created room recieved";
            emit created(id);
            PlayerData::id = id;
            isStarted = true;
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
        auto id = parametrs.get<int>("id");
        PlayerData::id = id;
        emit joined(id);
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
            emit deletePlayer(cit->id);
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
            emit drawPlayer(players.back().id);
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
        return;
    }

    if (command == "day") {
        return;
    }

    if (command == "nigth") {
        return;
    }

    if (command == "vote") {
        game_vote_answer(answer);
        return;
    }

    if (command == "game_room") {
        if (isStarted) {
            game_ping_answer(answer);
            return;
        }
    }
}

void Resolver::game_vote_answer(const boost::property_tree::ptree &answer) {
    emit hasVoted();
}

void Resolver::game_leave_answer(const boost::property_tree::ptree &answer) {

}


void Resolver::game_ping_answer(const boost::property_tree::ptree &answer) {
    if (isFirst) {
        emit startGame();
        isFirst = false;
    }
    auto parametrs = answer.get_child("parametrs");
    auto ids = parametrs.get<std::string>("ids");
    auto ips = parametrs.get<std::string>("ips");
    auto game = answer.get_child("game");

    auto id = parametrs.get<int>("id");
    auto role = game.get<int>("role");
    auto alive = game.get<std::string>("status_is_alive");
    auto sleep = game.get<std::string>("status_is_sleep");
    auto status = game.get<std::string>("status");
    auto status_iteration = game.get<std::string>("status_iteration");
    players.clear();
    if (status == "GAMING") {
        std::vector<std::string> ids_array = split(ids);
        std::vector<std::string> ips_array = split(ips);

        std::vector<resolver::Player> new_players;
        for (size_t i = 0; i < ids_array.size(); ++i) {
            resolver::Player player;
            std::vector<std::string> temp = split(ids_array[i], '-');
            player.id = std::stoi(temp[0]);
            player.is_live = (temp[1] == "ON" ? true : false);
            player.ip = ips_array[i];
            player.is_day = (status_iteration == "day" ? true : false);
            if (player.id == id) {

                PlayerData::role = role;
                PlayerData::isAlive = (alive == "ON" ? true : false);
                PlayerData::role = role;

                player.is_me = true;
                player.role = role;
                player.is_live = (alive == "ON" ? true : false);
                player.is_sleep = (sleep == "ON" ? true : false);
            } else {
                player.is_me = false;
            }
            players.push_back(player);
        }
        emit gameIteration(players);

    } else if (status == "CITIZEN_WIN") {
        if (role == 777) {
            emit citizenWin();
            return;
        }
        if (role == 2) {
            emit loseGame();
        } else if (role == 1) {
            emit winGame();
        }
    } else if (status == "MAFIA_WIN") {
        if (role == 777) {
            emit mafiaWin();
            return;
        }
        if (role == 2) {
            emit winGame();
        } else if (role == 1) {
            emit loseGame();
        }
    }
}

void Resolver::Run() {
    while (Client->isConnected()) {
        auto m = Client->getLastMsg();
        std::cout << m;
        std::stringstream msg(m);
        pt::ptree json_data;
        pt::read_json(msg, json_data);
        parse_answer(json_data);
    }
}

#include "moc_resolver.cpp" // !!!
