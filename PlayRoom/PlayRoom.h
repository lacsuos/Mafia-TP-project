#pragma once

#include <vector>
#include "Player.h"


class PlayRoom {
private:
    int roomSize_;
    int userCounter_;
    int mafiaCounter_;
    int citizenCounter_;
    std::vector<Player*> players_;
public:
    PlayRoom() = default;
    explicit PlayRoom(const std::vector<size_t> vecOfId);
    int CountingVotes(const std::vector<size_t> vecOfId);
    bool day();
    bool evening(const std::vector<size_t> vecOfId);
    void night(const std::vector<size_t> vecOfId);
    void kill(int userId);
    bool IsGameOver();
    void SleepAllCitizen();
    void WakeUpAll();
    int globalToRoom(int userId);
    int GetMafiaCounter();
    int GetCitizenCounter();
    ~PlayRoom();
};

