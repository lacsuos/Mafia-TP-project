#ifndef MAFIA_TP_PROJECT_GAMEHOST_H
#define MAFIA_TP_PROJECT_GAMEHOST_H
#include <string>


class GameHost {
private:
    size_t userId;
    std::string userName;
    std::string role;
public:
    void dayStage();
    void eveningStage();
    void nightStage();
};

#endif //MAFIA_TP_PROJECT_GAMEHOST_H
