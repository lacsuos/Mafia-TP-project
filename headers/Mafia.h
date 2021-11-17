#ifndef MAFIA_TP_PROJECT_PLAYER_H
#define MAFIA_TP_PROJECT_PLAYER_H
#include <string>
#include "Player.h"

class Mafia : public Player{
private:
    size_t userId;
    std::string userName;
    std::string role;
    bool isSleep;
    bool alive;
    bool isBoss;
public:
    virtual void roleAction();
};

#endif //MAFIA_TP_PROJECT_PLAYER_H
