#pragma once

#include <string>
#include "Player.h"

class Citizen : public Player {
public:
    Citizen();
    ~Citizen() override;
};
