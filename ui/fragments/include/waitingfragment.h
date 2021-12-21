#ifndef WAITINGFRAGMENT_H
#define WAITINGFRAGMENT_H

#include "abstract\include\abstractfragment.h"
#include "main\include\screensfactory.h"

#include <QPushButton>

class WaitingFragment: public AbstractFragment {
    Q_OBJECT
public:
    WaitingFragment();
    ~WaitingFragment();

public slots:
    void onBackPressed();
    void onContinuePressed();

private:
    QPushButton *BackButton;
     QPushButton *ContinueButton;
};

#endif // WAITINGFRAGMENT_H
