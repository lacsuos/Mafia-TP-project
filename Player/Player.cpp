#include "Player.h"

Player::Player(): globalId_(0), roomId_(0), alive_(true), isSleep_(false), role_(0) {}


void Player::setGlobalId(size_t newId) {
    globalId_ = newId;
}


void Player::setRoomId(size_t newId) {
    roomId_ = newId;
}


void Player::setAlive(bool newStatus) {
    alive_ = newStatus;
}


void Player::setIsSleep(bool newStatus) {
    isSleep_ = newStatus;
}


size_t Player::getGlobalId() {
    return globalId_;
}


size_t Player::getRoomId() {
    return roomId_;
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