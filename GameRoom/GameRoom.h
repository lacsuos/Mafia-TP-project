#pragma once

#include <vector>
#include "Player.h"


class GameRoom {
private:
    size_t roomSize_;
    size_t userCounter_;
    size_t mafiaCounter_;
    size_t citizenCounter_;
    std::vector<Player*> players_;
public:
    GameRoom();
    void PlayersInfo();
    size_t CountingVotes();
    bool day();
    bool evening();
    void night();
    void kill(size_t userId);
    bool IsGameOver();
    void SleepAllCitizen();
    void WakeUpAll();
    size_t GetMafiaCounter();
    size_t GetCitizenCounter();
    ~GameRoom();
};

