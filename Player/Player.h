#pragma once

#include <string>
#include <iostream>

class Player {
private:
    size_t userId_;
    bool alive_;
    bool isSleep_;
protected:
    int role_;
public:
    Player();
    size_t vote(); // только для консольки
    virtual size_t roleAction() = 0; // только для консольки
    void setUserId(size_t newId);
    void setAlive(bool newStatus);
    void setIsSleep(bool newStatus);
    size_t getUserId();
    bool getAlive();
    bool getIsSleep();
    int getRole();
    virtual ~Player();
};

