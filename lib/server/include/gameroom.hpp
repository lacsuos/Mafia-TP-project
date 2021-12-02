#pragma once

#include "user.hpp"
#include <string>

class GameRoom {
public:
    GameRoom() : is_game_connecting(false) {
        game_id = id++;
    };

    ~GameRoom() { users.clear(); }

    std::atomic<bool> is_full;

    void add_user(User* user) { users.push_back(user); }

    void delete_user(const std::shared_ptr<User> &user);

    std::atomic<bool> is_game_connecting;

    size_t get_id() const { return game_id; }

    size_t get_size() const { return users.size(); }

    User* get_user(size_t number) const { return users[number]; }

private:
    std::vector<User *> users;
    static size_t id;
    size_t game_id;
};

size_t GameRoom::id = 1;

void GameRoom::delete_user(const std::shared_ptr<User> &user) {
    auto it = std::find(users.begin(), users.end(), user);
    if (it != users.end()) {
        users.erase(it);
    }
}
