#include <gtest/gtest.h>
#include <iostream>
#include "GameProcess.h"

TEST(GameProcess, someoneWin) {
    GameProcess gameProcess;
    ASSERT_NO_THROW(gameProcess.someoneWin());
}

TEST(GameProcess, GameLoop) {
    GameProcess gameProcess;
    ASSERT_NO_THROW(gameProcess.GameLoop());
}
