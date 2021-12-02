#include "Player.h"

Player::Player(): userId(0), alive(true), isSleep(false) {}


size_t Player::vote() {
    size_t userVote = 0;
    std::cout << "Введите id игрока, за которого голосуете: " ;
    std::cin >> userVote;
    return userVote;
}


void Player::setUserId(size_t newId) {
    userId = newId;
}


void Player::setUserName(std::string newName) {
    userName = newName;
}


void Player::setAlive(bool newStatus) {
    alive = newStatus;
}


void Player::setIsSleep(bool newStatus) {
    isSleep = newStatus;
}