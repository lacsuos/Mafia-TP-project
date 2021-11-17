#ifndef MAFIA_TP_PROJECT_PLAYER_H
#define MAFIA_TP_PROJECT_PLAYER_H
#include <string>


class Player {
public:
    void vote(size_t userId);
    void sleep();
    void awake();
    virtual void roleAction();
};

#endif //MAFIA_TP_PROJECT_PLAYER_H
