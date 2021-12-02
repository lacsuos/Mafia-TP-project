#include "GameRoom.h"
#include <ctime>
#include <algorithm>
#include <random>
#include "Citizen.h"
#include "Mafia.h"

GameRoom::GameRoom(): roomSize(10), userCounter(0),
mafiaCounter(2), citizenCounter(8), players(roomSize) {
    size_t bufId = 0;
    std::cout << "Введите количество пользователей: " << std::endl;
    std::cin >> userCounter;
    int pl[] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    std::shuffle(std::begin(pl), std::end(pl), std::default_random_engine());
    for (size_t i = 0; i < userCounter; ++i) {
        std::cout << "Введите id пользователя: " << std::endl;
        std::cin >> bufId;

        auto Role = [](int a) -> Player * {
            switch (a) {
                case 0:
                    return new Citizen();
                case 1:
                    return new Mafia();
                default:
                    throw "BadRole";
            }
        };

        players[i] = Role(pl[i]);
        players[i]->setUserId(bufId);
    }
}


GameRoom::~GameRoom() {
    for (int i = 0; i < players.size(); ++i) {
        delete players[i];
    }
}


bool GameRoom::day() {
    return IsGameOver(mafiaCounter, citizenCounter);
}


bool GameRoom::evening() {
    kill(CountingVotes());
    return  IsGameOver(mafiaCounter, citizenCounter);
}


void GameRoom::night() {
    kill(CountingVotes());
}


void GameRoom::GameLoop() {
    while (true) {
        if (day())
            break;  // конец игры,
        if (evening())
            break; // подсчет голосов, дальше проверка на выбывание и убиваем игрока. Проверка на конец игры
        night();  // подсчет голосов, дальше проверка на выбывание и убиваем игрока
    }
}


void GameRoom::kill(size_t userId) {
    players[userId]->setAlive(false);
}


size_t GameRoom::CountingVotes() {
    std::cout << "подсчет" << std::endl;
    int counterArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /*while (query >> m_id)
        counterArray[m_id]++;*/
    int max = -1, counterMax = 0;
    size_t maxIndex = -1;
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
    if (counterMax > 1)
        return -1; // голосование неопределенное
    else
        return maxIndex; // голосование успешно
}


bool GameRoom::IsGameOver(size_t mafiaCounter, size_t citizenCounter) {
    if (mafiaCounter == 0) {
        std::cout << "Мирные победили" << std::endl;
        return true;
    }
    if (mafiaCounter >= citizenCounter) {
        std::cout << "Мафия победила" << std::endl;
        return true;
    }
    return false;
}
