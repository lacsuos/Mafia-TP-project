#pragma once

#include <string>
#include "Player.h"

class Citizen : public Player {
public:
    Citizen();
    size_t roleAction() override;
    ~Citizen() override;
};
