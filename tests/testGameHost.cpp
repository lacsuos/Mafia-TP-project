#include <gtest/gtest.h>
#include <iostream>
#include "GameHost.h"


void eveningStage();
void nightStage();

TEST(Player, dayStage) {
    GameHost host;
    ASSERT_NO_THROW(host.dayStage());
}

TEST(Player, eveningStage) {
    GameHost host;
    ASSERT_NO_THROW(host.eveningStage());
}

TEST(Player, nightStage) {
    GameHost host;
    ASSERT_NO_THROW(host.nightStage());
}
