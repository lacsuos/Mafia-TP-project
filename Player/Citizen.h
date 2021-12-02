#pragma once

#include <string>
#include "Player.h"

class Citizen : public Player {
public:
    size_t roleAction() override;
    ~Citizen() override;
};
