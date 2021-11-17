#ifndef MAFIA_TP_PROJECT_ROOMINFO_H
#define MAFIA_TP_PROJECT_ROOMINFO_H
#include <vector>

class RoomInfo {
private:
    size_t userCounter;
    size_t mafiaCounter;
    size_t citizenCounter;
public:
    void StartGame(size_t userCounter,
                   std::vector<std::string> usersNames,
                   std::vector<size_t> usersId);
    void giveRoles(std::vector<Player> playersVec);
    void GameOver(
            size_t mafiaCounter,
            size_t citizenCounter);
};

#endif //MAFIA_TP_PROJECT_ROOMINFO_H
