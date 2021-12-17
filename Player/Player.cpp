#include "Player.h"

Player::Player()
: globalId_(0)
, roomId_(0)
, isAlive_(true)
, isSleep_(false)
, role_(Role::NO_ROLE) {}


void Player::setGlobalId(int newId) {
    globalId_ = newId;
}


void Player::setRoomId(int newId) {
    roomId_ = newId;
}


void Player::setIsAlive(bool isAlive) {
    isAlive_ = isAlive;
}


void Player::setIsSleep(bool isSleep) {
    isSleep_ = isSleep;
}


int Player::getGlobalId() {
    return globalId_;
}


int Player::getRoomId() {
    return roomId_;
}


bool Player::getIsAlive() {
    return isAlive_;
}

bool Player::getIsSleep() {
    return isSleep_;
}

Role Player::getRole() {
    return role_;
}

Player::~Player() {}