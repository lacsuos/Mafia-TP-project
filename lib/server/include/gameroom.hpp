#pragma once

#include "user.hpp"
#include <string>

static size_t id_gameroom_generate = 1;

class GameRoom {
public:
    GameRoom(size_t admin_id_) : is_game_connecting(false),
                                 admin_id(admin_id_) { game_id = id_gameroom_generate++; }

    GameRoom() = delete;

    ~GameRoom() = default;

    std::atomic<bool> is_game_connecting;

    size_t get_id() const { return game_id; }

    size_t get_admin_id() const { return admin_id; }

private:
    size_t game_id;

    size_t admin_id;
};


