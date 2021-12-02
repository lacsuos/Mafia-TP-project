#ifndef MAFIA_TP_PROJECT_GAMEPROCESS_H
#define MAFIA_TP_PROJECT_GAMEPROCESS_H
#include "GameRoom.h"
#include "Queue.h"
#include "Player.h"
#include "GameHost.h"


class GameProcess {
private:
    std::vector<Player> playersVec;
    std::map <std::string, uint8_t> command_{
            {"vote", VOTE_SIGNAL},
            {"abstain", ABSTAIN_SIGNAL},
            {"wait", WAIT_SIGNAL}
    };

    bool someoneWin(size_t &citizenCounter, size_t &mafiaCounter);

    void GameLoop(std::vector<Player> playersVec,
                  size_t &citizenCounter,
                  size_t &mafiaCounter);

public:
    Queue <std::string> commandQueue;

};

#endif //MAFIA_TP_PROJECT_GAMEPROCESS_H
