#pragma once
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <QMessageBox>

#include "screensfactory.h"
#include "gamefragment.h"

namespace pt = boost::property_tree;

namespace resolver {

    struct Player {
        std::string name;
        int id;
        int role;
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


private:
    void ParseAnswer(pt::ptree const &answer);
    void base_answer(pt::ptree const &answer);

    void CreateRoomAnswer(pt::ptree const &answer);
    void JoinRoomAnswer(pt::ptree const &answer);

    void CheckPlayers(const std::vector<resolver::Player> &new_players);

private:
    int current_turn;
    bool first_msg;
    bool is_admin;
    bool is_started;
    std::vector<resolver::Player> players;
};
