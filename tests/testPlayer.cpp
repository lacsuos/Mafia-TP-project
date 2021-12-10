#include <gtest/gtest.h>
#include <iostream>
#include "Player.h"

TEST(Player, vote) {
    Player player;
    ASSERT_NO_THROW(player.vote(2));
}

TEST(Player, sleep) {
    Player player;
    ASSERT_NO_THROW(player.sleep());
}

TEST(Player, awake) {
    Player player;
    ASSERT_NO_THROW(player.awake());
}
