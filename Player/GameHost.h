#pragma once

#include <string>
#include "Player.h"

class GameHost : public Player {
public:
    GameHost();
    size_t roleAction() override;
    ~GameHost() override;
};
