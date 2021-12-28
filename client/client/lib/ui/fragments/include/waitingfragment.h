#ifndef WAITINGFRAGMENT_H
#define WAITINGFRAGMENT_H

#include "abstractfragment.h"


#include <QPushButton>
#include <QLabel>
#include <QMessageBox>


class WaitingFragment: public AbstractFragment {
    Q_OBJECT
public:
    WaitingFragment();
    ~WaitingFragment();

public slots:
    void onBackPressed();
    //void onContinuePressed();
    //void onDisconnectPressed();
    void onGameStarts();
    void onDrawPlayer(int id);
    void onDeletePlayer(int id);

private:
    QPushButton *backButton;
    QString playersStr;
    QLabel *roomID;
    QLabel *playersCounter;
    //QPushButton *continueButton;
    //QLabel *Players;
    //QPushButton *DisconnectButton;
};

#endif // WAITINGFRAGMENT_H
