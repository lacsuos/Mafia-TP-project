#pragma once

#include <string>
#include <iostream>

class Player {
private:
    size_t globalId_;
    size_t roomId_;
    bool alive_;
    bool isSleep_;
protected:
    int role_;
public:
    Player();
    void setGlobalId(size_t newId);
    void setRoomId(size_t newId);
    void setAlive(bool newStatus);
    void setIsSleep(bool newStatus);
    size_t getGlobalId();
    size_t getRoomId();
    bool getAlive();
    bool getIsSleep();
    int getRole();
    virtual ~Player();
};

