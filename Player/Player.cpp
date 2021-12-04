#include "Player.h"

Player::Player(): userId_(0), alive_(true), isSleep_(false), role_(0) {}


size_t Player::vote() {
    size_t userVote = 0;
    std::cout << "Введите id игрока, за которого голосуете: " ;
    std::cin >> userVote;
    return userVote;
}


void Player::setUserId(size_t newId) {
    userId_ = newId;
}


void Player::setAlive(bool newStatus) {
    alive_ = newStatus;
}


void Player::setIsSleep(bool newStatus) {
    isSleep_ = newStatus;
}


size_t Player::getUserId() {
    return userId_;
}

bool Player::getAlive() {
    return alive_;
}

bool Player::getIsSleep() {
    return isSleep_;
}

int Player::getRole() {
    return role_;
}

Player::~Player() {}