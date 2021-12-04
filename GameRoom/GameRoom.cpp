#include "GameRoom.h"
#include <ctime>
#include <algorithm>
#include <random>
#include "Citizen.h"
#include "Mafia.h"
#include "GameHost.h"

GameRoom::GameRoom(): roomSize_(10), userCounter_(10),
mafiaCounter_(2), citizenCounter_(7), players_(roomSize_) {
    size_t bufId = 0;
    int pl[] = {1, 1, 777, 0, 0, 0, 0, 0, 0, 0};
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng(seed);
    std::shuffle(std::begin(pl), std::end(pl), eng);
    for (size_t i = 0; i < userCounter_; ++i) {
        std::cout << "Введите id пользователя: " << std::endl;
        std::cin >> bufId;
        auto Role = [](int a) -> Player * {
            switch (a) {
                case 0:
                    return new Citizen();
                case 1:
                    return new Mafia();
                case 777:
                    return new GameHost();
                default:
                    throw "BadRole";
            }
        };

        players_[i] = Role(pl[i]);
        players_[i]->setUserId(bufId);
    }
}


GameRoom::~GameRoom() {
    for (int i = 0; i < players_.size(); ++i) {
        delete players_[i];
    }
}


bool GameRoom::day() {
    WakeUpAll();
    return IsGameOver();
}


bool GameRoom::evening() {
    size_t resultOfVoting = CountingVotes();
    if (resultOfVoting != -1)
        kill(resultOfVoting);
    return  IsGameOver();
}


void GameRoom::night() {
    SleepAllCitizen();
    size_t resultOfVoting = CountingVotes();
    if (resultOfVoting != -1)
        kill(resultOfVoting);
}


void GameRoom::kill(size_t userId) {
    std::cout << "kill " << userId << std::endl;
    players_[userId]->setAlive(false);
    if (players_[userId]->getRole() == 1) {
        citizenCounter_--;
    }
    if (players_[userId]->getRole() == 2) {
        mafiaCounter_--;
    }
}


size_t GameRoom::CountingVotes() {
    std::cout << "подсчет" << std::endl;
    int counterArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int voteArray[10] = {4, 1, 1, 5, 1, 1, 4, 5, 1, 4};
    int max = -1, counterMax = 0;
    size_t maxIndex = -1;
    for (size_t i = 0; i < 10; ++i) {
        counterArray[voteArray[i]]++;
    }
    for (size_t i = 0; i < 10; ++i) {
        if (max == counterArray[i]) {
            counterMax++;
        }
        if (max < counterArray[i]) {
            max = counterArray[i];
            maxIndex = i;
            counterMax = 1;
        }
    }
    std::cout << "maxIndex= " << maxIndex << std::endl;
    if (counterMax > 1)
        return -1; // голосование неопределенное
    else
        return maxIndex; // голосование успешно
}


bool GameRoom::IsGameOver() {
    std::cout << "IsGameOver: " << mafiaCounter_ << " " << citizenCounter_ << std::endl;
    if (mafiaCounter_ == 0) {
        std::cout << "Мирные победили" << std::endl;
        return true;
    }
    if (mafiaCounter_ >= citizenCounter_) {
        std::cout << "Мафия победила" << std::endl;
        return true;
    }
    return false;
}


size_t GameRoom::GetMafiaCounter() {
    return mafiaCounter_;
}


size_t GameRoom::GetCitizenCounter() {
    return citizenCounter_;
}

void GameRoom::PlayersInfo() {
    for (int i = 0; i < 10; ++i) {
        std::cout << "PlayerID: " << players_[i]->getUserId() << std::endl;
        std::cout << "Role: " << players_[i]->getRole() << std::endl;
    }
}


void GameRoom::SleepAllCitizen() {
    for (int i = 0; i < 10; ++i) {
        if ( players_[i]->getRole() == 1)
            players_[i]->setIsSleep(true);
    }
}


void GameRoom::WakeUpAll() {
    for (int i = 0; i < 10; ++i) {
        players_[i]->setIsSleep(false);
    }
}