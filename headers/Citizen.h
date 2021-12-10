#ifndef MAFIA_TP_PROJECT_PLAYER_H
#define MAFIA_TP_PROJECT_PLAYER_H
#include <string>
#include "Player.h"

class Citizen : public Player{
private:
    size_t userId;
    std::string userName;
    std::string role;
    bool isSleep;
    bool alive;
};

#endif //MAFIA_TP_PROJECT_PLAYER_H
