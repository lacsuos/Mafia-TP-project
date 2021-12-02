#pragma once

#include <string>
#include <iostream>

class Player {
private:
    size_t userId;
    bool alive;
    bool isSleep;
public:
    Player();
    size_t vote(); // только для консольки
    virtual size_t roleAction(); // только для консольки
    void setUserId(size_t newId);
    void setAlive(bool newStatus);
    void setIsSleep(bool newStatus);
    void getUserId();
    void getAlive();
    void getIsSleep();
    virtual ~Player();
};

