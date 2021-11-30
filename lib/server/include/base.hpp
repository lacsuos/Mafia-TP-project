#pragma once

#include "base_queue.hpp"
#include "connection.hpp"

namespace net {
    struct Base {
        BaseQueue<std::shared_ptr<Connection>> creating_game;
    };
}