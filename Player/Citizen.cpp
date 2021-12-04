#include "Citizen.h"

Citizen::Citizen() {
    role_ = 1;
}

size_t Citizen::roleAction() {
    std::cout << "fff";
    return 777;
}

Citizen::~Citizen() noexcept {
}