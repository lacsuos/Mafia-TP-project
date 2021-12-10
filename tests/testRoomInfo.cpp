#include <gtest/gtest.h>
#include <iostream>
#include "PlayRoom.h"

TEST(PlayRoom, Init) {
    std::vector<int> vecOfId = {111, 222, 333, 444};
    ASSERT_NO_THROW(PlayRoom room(vecOfId));
}


TEST(PlayRoom, day) {
    std::vector<int> vecOfId = {111, 222, 333, 444};
    PlayRoom room(vecOfId);
    EXPECT_EQ(room.day(), false);
}

TEST(PlayRoom, evening) {
    std::vector<int> vecOfId = {111, 222, 333, 444};
    std::vector<int> vecOfVotes = {111, 111, 111, 444};
    PlayRoom room(vecOfId);
    EXPECT_EQ(room.evening(vecOfVotes), false);
}

TEST(PlayRoom, nighy) {
    std::vector<int> vecOfId = {111, 222, 333, 444};
    std::vector<int> vecOfVotes = {111, 111, 111, 444};
    PlayRoom room(vecOfId);
    EXPECT_EQ(room.night(vecOfVotes), 111);
}
