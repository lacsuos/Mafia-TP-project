#ifndef WAITINGFRAGMENT_H
#define WAITINGFRAGMENT_H

#include "abstractfragment.h"


#include <QPushButton>

class WaitingFragment: public AbstractFragment {
    Q_OBJECT
public:
    WaitingFragment();
    ~WaitingFragment();

public slots:
    void onBackPressed();
    void onContinuePressed();
    //void onDisconnectPressed();

private:
    QPushButton *BackButton;
    QPushButton *ContinueButton;
    //QLabel *Players;
    //QPushButton *DisconnectButton;
};

#endif // WAITINGFRAGMENT_H
