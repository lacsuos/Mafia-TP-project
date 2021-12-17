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
    PlayRoom room(vecOfId);
    const std::vector<std::unique_ptr<Player>>& plr = room.GetPlayers();
    std::vector<int> vecOfVotes = {0, 0, 0, 0};
    if (plr[0]->getRole() == Role::CITIZEN || plr[0]->getRole() == Role::MAFIA)
        vecOfVotes = {111, 111, 111, 444};
    else if (plr[0]->getRole() == Role::HOST)
        vecOfVotes = {222, 222, 222, 444};
    EXPECT_EQ(room.evening(vecOfVotes), true);
}

TEST(PlayRoom, night) {
    std::vector<int> vecOfId = {111, 222, 333, 444};
    std::vector<int> vecOfVotes = {111, 111, 111, 444};
    PlayRoom room(vecOfId);
    EXPECT_EQ(room.night(vecOfVotes), 111);
}
