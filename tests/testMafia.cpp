#include "Mafia.h"

TEST(Mafia, vote) {
Mafia mafia;
ASSERT_NO_THROW(mafia.roleAction());
}