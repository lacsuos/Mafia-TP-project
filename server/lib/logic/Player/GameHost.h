#pragma once

#include <string>
#include "Player.h"

class GameHost : public Player {
public:
    GameHost();
    ~GameHost() override;
};
