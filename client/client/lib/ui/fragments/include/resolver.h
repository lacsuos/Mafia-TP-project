#pragma once
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <QMessageBox>

#include "screensfactory.h"
#include "client.h"
#include "client_impl.h"

namespace pt = boost::property_tree;

namespace resolver {

    struct Player {
        std::string name;
        int id;
        int role;
        std::string ip;
        bool is_me;
        bool is_live;
        bool is_sleep;
        bool is_win;
    };
}


class Resolver : public AbstractFragment {
    Q_OBJECT
public:
    Resolver();
    ~Resolver() = default;
    void Run();

    signals:
    void DeleteAllPlayers();
    void DeletePlayer(int player_id);
    void DrawPlayer(int player_id, std::string nickname);
    void ShowStart();

    void serverDisconnected();
    void netError();
    void created();
    void joined();

private:
    void parse_answer(pt::ptree const &answer);
    void base_answer(pt::ptree const &answer);
    void base_room_answer(pt::ptree const &answer);

    void create_room_answer(pt::ptree const &answer);
    void join_room_answer(pt::ptree const &answer);
    void pregame_answer(pt::ptree const &answer);

    void game_vote_answer(pt::ptree const &answer);
    void game_leave_answer(pt::ptree const &answer);
    void game_finish_answer(pt::ptree const &answer);
    void game_day_answer(pt::ptree const &answer);
    void game_ping_answer(pt::ptree const &answer);
    void game_nigth_answer(pt::ptree const &answer);

    void check_players(const std::vector<resolver::Player> &new_players);

private:
    bool is_admin;
    bool is_started;
    std::vector<resolver::Player> players;
};


