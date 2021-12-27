#pragma once

#include "client.h"

inline std::unique_ptr<net::Client> Client;


struct PlayerData {
    static int id;
    static int room_id;
    static int role;
    static bool is_alive;

    static void reset() {
        id = 0;
        room_id = 0;
        role = 0;
        is_alive = 0;
    }
};

