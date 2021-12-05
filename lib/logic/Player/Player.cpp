#include "Player.h"

Player::Player(): globalId_(0), roomId_(0), alive_(true), isSleep_(false), role_(0) {}


void Player::setGlobalId(int newId) {
    globalId_ = newId;
}


void Player::setRoomId(int newId) {
    roomId_ = newId;
}


void Player::setAlive(bool newStatus) {
    alive_ = newStatus;
}


void Player::setIsSleep(bool newStatus) {
    isSleep_ = newStatus;
}


int Player::getGlobalId() {
    return globalId_;
}


int Player::getRoomId() {
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