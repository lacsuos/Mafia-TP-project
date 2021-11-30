#pragma once

#include "user.hpp"

namespace net {
    class GameRoom {
    public:
        GameRoom() {
            game_id = id++;
        };

        ~GameRoom() { users.clear(); }

        std::atomic<bool> is_full;

        void addUser(const std::shared_ptr<User> &user) {
            users.push_back(user);
        }

        void deleteUser(const std::shared_ptr<User> &user) {
            auto it = std::find(users.begin(), users.end(), user);
            if (it != users.end()) {
                users.erase(it);
            }
        }

    private:
        std::vector<std::shared_ptr<User>> users;
        static size_t id;
        size_t game_id;
    };

    size_t GameRoom::id = 1;
}