#pragma once
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <QMessageBox>
#include <QDebug>
#include <abstractfragment.h>
#include "client.h"
#include "vector"
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
        bool is_day;
    };
    static inline std::vector<Player> savedPlayers;
}



class Resolver : public AbstractFragment {
    Q_OBJECT
public:
    Resolver();
    ~Resolver() = default;
    void Run();

    signals:
    void hasVoted();
    void DeleteAllPlayers();
    void startGame();
    void deletePlayer(int player_id);
    void drawPlayer(int player_id);
    void gameIteration();
    void playerLeft();

    void serverDisconnected();
    void netError();
    void created(const int &idRoom);
    void joined(const int &idRoom);
    void mafiaWin();
    void citizenWin();
    void winGame();
    void loseGame();

private:
    void parse_answer(pt::ptree const &answer);
    void base_answer(pt::ptree const &answer);
    void base_room_answer(pt::ptree const &answer);

    void create_room_answer(pt::ptree const &answer);
    void join_room_answer(pt::ptree const &answer);
    void pregame_answer(pt::ptree const &answer);

    void game_vote_answer(pt::ptree const &answer);
    void game_leave_answer(pt::ptree const &answer);
    void game_ping_answer(pt::ptree const &answer);

    void check_players(const std::vector<resolver::Player> &new_players);

private:
    bool isFirst;
    bool isAdmin;
    bool isStarted;
    std::vector<resolver::Player> players;
};


