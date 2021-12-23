#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"

#include <QPushButton>
#include <QLabel>

class GameFragment: public AbstractFragment {
    Q_OBJECT
public:
    GameFragment();
    ~GameFragment();


public slots:
    void onBackPressed();
    //void addPlayers();
    void setDayState();
    void setNightState();
    void setCustomState(const QString state);


private:
    QPushButton *BackButton;
    QLabel *StateLabel;
};

#endif // GAMEFRAGMENT_H
