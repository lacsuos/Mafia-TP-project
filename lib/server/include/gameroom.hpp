#pragma once

#include "user.hpp"
#include <string>

class GameRoom {
public:
    GameRoom(size_t admin_id_) : is_game_connecting(false),
                                 admin_id(admin_id_) { game_id = id++; }

    GameRoom() = delete;

    ~GameRoom() = default;

    std::atomic<bool> is_game_connecting;

    size_t get_id() const { return game_id; }

    size_t get_admin_id() const { return admin_id; }

private:
    static size_t id;
    size_t game_id;

    size_t admin_id;
};

size_t GameRoom::id = 1;


