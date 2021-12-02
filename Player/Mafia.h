#pragma once

#include <string>
#include "Player.h"

class Mafia: public Player {
public:
    size_t roleAction() override;
    ~Mafia() override;
};
