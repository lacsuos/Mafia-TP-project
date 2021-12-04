#include "GameHost.h"

GameHost::GameHost() {
    role_ = 777;
}

size_t GameHost::roleAction() {
    std::cout << "fff";
    return 777;
}

GameHost::~GameHost() noexcept {
}