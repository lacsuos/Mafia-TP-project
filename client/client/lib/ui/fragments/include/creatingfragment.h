#ifndef CREATINGFRAGMENT_H
#define CREATINGFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"

#include <QPushButton>
#include <QLabel>


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
    QPushButton *backButton;
    QLabel *roomID;
    QLabel *playersCount;
    //QPushButton *StartButton
    //QPushButton *cancelButton
};


#endif // CREATINGFRAGMENT_H
