#ifndef CREATINGFRAGMENT_H
#define CREATINGFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"

#include <QPushButton>

class CreatingFragment: public AbstractFragment {
    Q_OBJECT
public:
    CreatingFragment();
    ~CreatingFragment();

public slots:
    void onBackPressed();
    //void onStartPressed();
    //void onCancelPressed();

private:
    QPushButton *BackButton;
    //QLabel *Players;
    //QPushButton *StartButton
    //QPushButton *CancelButton
};


#endif // CREATINGFRAGMENT_H
