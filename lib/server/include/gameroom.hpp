#pragma once

#include "user.hpp"
#include <string>

static int id_gameroom_generate = 1;

class GameRoom {
public:
    GameRoom(size_t admin_id_) : admin_id(admin_id_) { game_id = id_gameroom_generate++; }

    GameRoom() = delete;

    ~GameRoom() = default;

    int get_id() const { return game_id; }

    int get_admin_id() const { return admin_id; }

private:
    int game_id;

    int admin_id;
};


