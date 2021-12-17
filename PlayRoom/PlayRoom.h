#pragma once

#include <memory>
#include <vector>
#include "Player.h"


class PlayRoom {
private:
    int roomSize_;

    int userCounter_;

    int mafiaCounter_;

    int citizenCounter_;

    std::vector<std::unique_ptr<Player>> players_;

public:
    PlayRoom() = default;

    PlayRoom(const PlayRoom& room) = default;

    PlayRoom& operator=(const PlayRoom& room) = default;

    // возвращаем вектор всех игроков
    const std::vector<std::unique_ptr<Player>>& GetPlayers();

    // создаем комнату на N человек, раздаем каждому роль
    explicit PlayRoom(const std::vector<int>& vecOfId);

    // подсчет голосов: вернет id игрока, за которого проголосовало большинство
    // если нет такого игрока - вернет -1
    int CountingVotes(const std::vector<int>& vecOfId);

    // вернет false, если игра продолжается, и true, если игра закончена
    bool day();

    // вернет false, если игра продолжается, и true, если игра закончена
    bool evening(const std::vector<int>& vecOfId);

    // вернет id игрока, которого убили
    int night(const std::vector<int>& vecOfId);

    // меняет статус игрока, которого убили, на мертвого
    void kill(int userId);

    // вернет false, если игра продолжается, и true, если игра закончена
    bool IsGameOver();

    // переводит статус всех мирных в сон
    void SleepAllCitizen();

    // все живые мирные просыпаются
    void WakeUpAll();

    // переводит глобальный id в локальный
    int globalToRoom(int userId);

    // переводит локальный id в глобальный
    int roomToGlobal(int userId);

    // получить количество мафии
    int GetMafiaCounter();

    // получить количество мирных
    int GetCitizenCounter();

    // получить игрока
    Player GetPlayer(int userID);

    ~PlayRoom() = default;
};

