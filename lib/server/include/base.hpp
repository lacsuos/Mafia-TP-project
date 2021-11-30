#pragma once

#include "base_queue.hpp"
#include "user.hpp"

namespace net {
    struct Base {
        BaseQueue<std::shared_ptr<User>> creating_game;
    };
}