#include "PlayRoom.h"
#include <ctime>
#include <algorithm>
#include <random>
#include "Citizen.h"
#include "Mafia.h"
#include "GameHost.h"

 PlayRoom::PlayRoom(const std::vector<int> vecOfId): roomSize_(10), userCounter_(10),
mafiaCounter_(2), citizenCounter_(7), players_(roomSize_) {
    int pl[] = {2, 2, 777, 1, 1, 1, 1, 1, 1, 1};
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng(seed);
    std::shuffle(std::begin(pl), std::end(pl), eng);
    for (int i = 0; i < userCounter_; ++i) {
        auto Role = [](int a) -> Player * {
            switch (a) {
                case 1:
                    return new Citizen();
                case 2:
                    return new Mafia();
                case 777:
                    return new GameHost();
                default:
                    throw "BadRole";
            }
        };

        players_[i] = Role(pl[i]);
        players_[i]->setGlobalId(vecOfId[i]);
        players_[i]->setRoomId(i);
    }
}


PlayRoom::~PlayRoom() {
    for (size_t i = 0; i < players_.size(); ++i) {
        delete players_[i];
    }
}


bool PlayRoom::day() {
    WakeUpAll();
    return IsGameOver();
}


bool PlayRoom::evening(const std::vector<int> vecOfId) {
    int resultOfVoting = CountingVotes(vecOfId);
    if (resultOfVoting != -1)
        kill(resultOfVoting);
    return  IsGameOver();
}


int PlayRoom::night(const std::vector<int> vecOfId) {
    SleepAllCitizen();
    int resultOfVoting = CountingVotes(vecOfId);
    if (resultOfVoting != -1)
        kill(resultOfVoting);
    return resultOfVoting;
}


void PlayRoom::kill(int userId) {
    int roomId = globalToRoom(userId);
    players_[roomId]->setAlive(false);
    if (players_[roomId]->getRole() == 1) {
        citizenCounter_--;
    }
    if (players_[roomId]->getRole() == 2) {
        mafiaCounter_--;
    }
}

int PlayRoom::globalToRoom(int userId) {
    for (int i = 0; i < roomSize_; ++i) {
        if (players_[i]->getGlobalId() == userId)
            return players_[i]->getRoomId();
    }
    return -1;
}

int PlayRoom::roomToGlobal(int userId) {
    {
        for (int i = 0; i < roomSize_; ++i) {
            if (players_[i]->getRoomId() == userId)
                return players_[i]->getGlobalId();
        }
        return -1;
    }
}


int PlayRoom::CountingVotes(const std::vector<int> vecOfId) {
    int counterArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int voteArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (size_t i = 0; i < players_.size(); ++i) {
        voteArray[i] = globalToRoom(vecOfId[i]);
        if (voteArray[i] == -1)
            throw "BadID";
    }
    int max = -1, counterMax = 0;
    int maxIndex = -1;
    for (int i = 0; i < roomSize_; ++i) {
        counterArray[voteArray[i]]++;
    }
    for (int i = 0; i < roomSize_; ++i) {
        if (max == counterArray[i]) {
            counterMax++;
        }
        if (max < counterArray[i]) {
            max = counterArray[i];
            maxIndex = i;
            counterMax = 1;
        }
    }
    if (counterMax > 1)
        return -1; // голосование неопределенное
    else
        return roomToGlobal(maxIndex);
}


bool PlayRoom::IsGameOver() {
    std::cout << "IsGameOver: " << mafiaCounter_ << " " << citizenCounter_ << std::endl;
    if (mafiaCounter_ == 0) {
        return true;
    }
    if (mafiaCounter_ >= citizenCounter_) {
        return true;
    }
    return false;
}


int PlayRoom::GetMafiaCounter() {
    return mafiaCounter_;
}


int PlayRoom::GetCitizenCounter() {
    return citizenCounter_;
}


void PlayRoom::SleepAllCitizen() {
    for (int i = 0; i < roomSize_; ++i) {
        if ( players_[i]->getRole() == 1)
            players_[i]->setIsSleep(true);
    }
}


std::vector<Player*> PlayRoom::GetPlayers() {
    return players_;
}


void PlayRoom::WakeUpAll() {
    for (int i = 0; i < roomSize_; ++i) {
        if (players_[i]->getAlive() == true)
            players_[i]->setIsSleep(false);
    }
}