#include "resolver.h"
#include "client.h"
#include "client_impl.h"

#include <algorithm>
#include <QDebug>
#include <QApplication>

using namespace screens;

constexpr int MAX_PLAYERS = 4;

Resolver::Resolver() : current_turn(MAX_PLAYERS), first_msg(true), is_admin(false),
                       is_started(false) {}

void Resolver::ParseAnswer(pt::ptree const &answer) {
    auto command_type = answer.get<std::string>("command_type");

    if (command_type == "ping") {
        return;
    }

    if (command_type == "error") {
        return;
    }

    if (command_type == "disconnect") {
//        disconnect(answer);
        return;
    }

    if (command_type == "basic") {
        base_answer(answer);
        return;
    }
}

void Resolver::base_answer(pt::ptree const &answer) {
    auto command = answer.get<std::string>("command");

    if (command == "create_room") {
        CreateRoomAnswer(answer);
        return;
    }

    if (command == "join_room") {
        JoinRoomAnswer(answer);
        return;
    }

}

void Resolver::CreateRoomAnswer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto status = parametrs.get<std::string>("status");

    if (status == "in_process") {
        return;
    }

    if (status == "done") {
//
        return;
    }

    if (status == "fail") {
        // error
    }
}

void Resolver::JoinRoomAnswer(pt::ptree const &answer) {
    auto parametrs = answer.get_child("parametrs");
    auto status = parametrs.get<std::string>("status");

    if (status == "in_process") {
        return;
    }

    if (status == "done") {
//
        return;
    }

    if (status == "fail") {
//
    }
}

void Resolver::CheckPlayers(const std::vector<resolver::Player> &new_players) {
    for (size_t i = 0; i < players.size(); ++i) {
        auto cit = players.cbegin() + i;
        auto res = std::find_if(new_players.begin(), new_players.end(),
                                [cit](const resolver::Player &current) { return current.id == cit->id; });
        if (res == new_players.end()) {
//            emit DeletePlayer(cit->position);
            players.erase(cit);
        }
    }

    for (auto &it : new_players) {
        auto res = std::find_if(players.begin(), players.end(),
                                [it](const resolver::Player &current) { return current.id == it.id; });
        if (res == players.end()) {
            players.push_back(it);
//            emit DrawPlayer();
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
        ParseAnswer(json_data);
    }
}
