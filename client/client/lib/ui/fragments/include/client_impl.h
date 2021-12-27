#pragma once

#include "client.h"

inline std::unique_ptr<net::Client> Client;


namespace PlayerData {
    inline int id;
    inline int roomID;
    inline int role;
    inline bool isAlive;
    inline bool isDay;

    inline static void reset() {
        id = 0;
        roomID = 0;
        role = 0;
        isAlive = false;
        isDay = false;
    }
}

