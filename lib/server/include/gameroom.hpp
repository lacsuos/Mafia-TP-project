#pragma once
#include "user.hpp"

namespace net {
    class GameRoom {
        std::atomic<bool> is_full;
    private:
        std::vector <std::shared_ptr<User>> users;
        size_t room_id;
    };
}