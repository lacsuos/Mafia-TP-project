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
    void setGlobalId(int newId);
    void setRoomId(int newId);
    void setAlive(bool newStatus);
    void setIsSleep(bool newStatus);
    int getGlobalId();
    int getRoomId();
    bool getAlive();
    bool getIsSleep();
    int getRole();
    virtual ~Player();
};

