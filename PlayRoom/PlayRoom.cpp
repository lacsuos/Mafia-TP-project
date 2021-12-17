#include "PlayRoom.h"
#include <ctime>
#include <algorithm>
#include <random>
#include "Citizen.h"
#include "Mafia.h"
#include "GameHost.h"

 static std::unique_ptr<Player> ChooseRole(int a) {
    switch (a) {
        case 1:
            return std::make_unique<Citizen> ();
        case 2:
            return std::make_unique<Mafia> ();
        case 777:
            return std::make_unique<GameHost> ();
        default:
            throw "BadRole";
    }
}


 PlayRoom::PlayRoom(const std::vector<int>& vecOfId)
 : roomSize_(vecOfId.size())
 , userCounter_(vecOfId.size())
 , mafiaCounter_(1)
 , citizenCounter_(2)
 , players_(roomSize_)
 {
    int pl[] = {2, 1, 777, 1};
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng(seed);
    std::shuffle(std::begin(pl), std::end(pl), eng);

    for (int i = 0; i < userCounter_; ++i) {
        players_[i] = ChooseRole(pl[i]);
        players_[i]->setGlobalId(vecOfId[i]);
        players_[i]->setRoomId(i);
    }
}


bool PlayRoom::day() {
    WakeUpAll();
    return IsGameOver();
}


bool PlayRoom::evening(const std::vector<int>& vecOfId) {
    int resultOfVoting = CountingVotes(vecOfId);
    if (resultOfVoting != -1)
        kill(resultOfVoting);
    return  IsGameOver();
}


int PlayRoom::night(const std::vector<int>& vecOfId) {
    SleepAllCitizen();
    int resultOfVoting = CountingVotes(vecOfId);
    if (resultOfVoting != -1)
        kill(resultOfVoting);
    return resultOfVoting;
}


void PlayRoom::kill(int userId) {
    int roomId = globalToRoom(userId);
    players_[roomId]->setIsAlive(false);

    if (players_[roomId]->getRole() == Role::CITIZEN) {
        citizenCounter_--;
    }

    if (players_[roomId]->getRole() == Role::MAFIA) {
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


int PlayRoom::CountingVotes(const std::vector<int>& vecOfId) {
    int counterArray[4] = {0, 0, 0, 0};
    int voteArray[4] = {0, 0, 0, 0};

    for (size_t i = 0; i < vecOfId.size(); ++i) {
        voteArray[i] = globalToRoom(vecOfId[i]);
        if (voteArray[i] == -1)
            throw "BadID";
    }

    int max = -1, counterMax = 0;
    int maxIndex = -1;

    for (size_t i = 0; i < vecOfId.size(); ++i) {
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
        if ( players_[i]->getRole() == Role::CITIZEN)
            players_[i]->setIsSleep(true);
    }
}


const std::vector<std::unique_ptr<Player>>& PlayRoom::GetPlayers() {
    return players_;
}


void PlayRoom::WakeUpAll() {
    for (int i = 0; i < roomSize_; ++i) {
        if (players_[i]->getIsAlive())
            players_[i]->setIsSleep(false);
    }
}


Player PlayRoom::GetPlayer(int userID) {
    for (size_t i = 0; i < players_.size(); ++i) {
        if (players_[i]->getRoomId() == globalToRoom(userID))
            return *players_[i];
    }
    throw "BadId";
}