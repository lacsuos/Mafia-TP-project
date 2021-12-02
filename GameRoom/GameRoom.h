#pragma once

#include <vector>
#include "Player.h"


class GameRoom {
private:
    size_t roomSize;
    size_t userCounter;
    size_t mafiaCounter;
    size_t citizenCounter;
    std::vector<Player*> players;
public:
    GameRoom();
    size_t CountingVotes();
    bool day();
    bool evening();
    void night();
    void kill(size_t userId);
    void GameLoop();
    void DealRols();
    bool IsGameOver(size_t mafiaCounter, size_t citizenCounter);
    ~GameRoom();
};

