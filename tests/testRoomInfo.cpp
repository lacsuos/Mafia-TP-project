#include <gtest/gtest.h>
#include <iostream>
#include "RoomInfo.h"

TEST(RoomInfo, StartGame) {
    RoomInfo roomInfo;
    ASSERT_NO_THROW(roomInfo.StartGame(2));
}

TEST(RoomInfo, giveRoles) {
    RoomInfo roomInfo;
    ASSERT_NO_THROW(roomInfo.giveRoles());
}

TEST(RoomInfo, GameOver) {
    RoomInfo roomInfo;
    ASSERT_NO_THROW(roomInfo.GameOver());
}
