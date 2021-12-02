#pragma once

#include "base_queue.hpp"
#include "communication.hpp"

namespace net {
    struct Base {
        BaseQueue<std::shared_ptr<Communication>> creating_game;
        BaseQueue<std::shared_ptr<Communication>> accepting_game;
    };
}