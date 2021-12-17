#pragma once

#include <string>
#include <iostream>

enum class Role {
    MAFIA,
    CITIZEN,
    HOST,
    NO_ROLE
};

class Player {
private:
    int globalId_;

    int roomId_;

    bool isAlive_;

    bool isSleep_;

protected:
    Role role_;
public:
    Player();

    // установить глобальный id
    void setGlobalId(int newId);

    // установить комнатный id
    void setRoomId(int newId);

    // установить статус Жив/Мертв
    void setIsAlive(bool isAlive);

    // установить статус Спит/Не спит
    void setIsSleep(bool isSleep);

    // получить глобальный id
    int getGlobalId();

    // получить комнатный id
    int getRoomId();

    // получить статус Жив/Мертв
    bool getIsAlive();

    // получить статус Спит/Не спит
    bool getIsSleep();

    // получить роль
    Role getRole();

    virtual ~Player();
};

