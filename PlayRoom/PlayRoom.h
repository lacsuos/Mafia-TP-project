#pragma once

#include <vector>
#include "Player.h"


class PlayRoom {
private:
    size_t roomSize_;
    size_t userCounter_;
    size_t mafiaCounter_;
    size_t citizenCounter_;
    std::vector<Player*> players_;
public:
    PlayRoom() = default;
    explicit PlayRoom(const std::vector<size_t> vecOfId);
    size_t CountingVotes(const std::vector<size_t> vecOfId);
    bool day();
    bool evening(const std::vector<size_t> vecOfId);
    void night(const std::vector<size_t> vecOfId);
    void kill(size_t userId);
    bool IsGameOver();
    void SleepAllCitizen();
    void WakeUpAll();
    size_t globalToRoom(size_t userId);
    size_t GetMafiaCounter();
    size_t GetCitizenCounter();
    ~PlayRoom();
};

