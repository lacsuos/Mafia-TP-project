#pragma once

#include "client.h"

inline std::unique_ptr<net::Client> Client;


struct PlayerData {
    static int id;
    static int roomID;
    static int role;
    static bool isAlive;
    static bool isDay;

    static void reset() {
        id = 0;
        roomID = 0;
        role = 0;
        isAlive = 0;
        isDay = true;
    }
};

