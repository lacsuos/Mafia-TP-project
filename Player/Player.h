#pragma once

#include <string>
#include <iostream>

class Player {
private:
    int globalId_;
    int roomId_;
    bool alive_;
    bool isSleep_;
protected:
    int role_;
public:
    Player();
    // установить глобальный id
    void setGlobalId(int newId);
    // установить комнатный id
    void setRoomId(int newId);
    // установить статус Жив/Мертв
    void setAlive(bool newStatus);
    // установить статус Спит/Не спит
    void setIsSleep(bool newStatus);
    // получить глобальный id
    int getGlobalId();
    // получить комнатный id
    int getRoomId();
    // получить статус Жив/Мертв
    bool getAlive();
    // получить статус Спит/Не спит
    bool getIsSleep();
    // получить роль
    int getRole();
    virtual ~Player();
};

