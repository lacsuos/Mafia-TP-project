#ifndef CREATINGFRAGMENT_H
#define CREATINGFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"

#include <QPushButton>
#include <QLabel>
#include <QMessageBox>


class CreatingFragment: public AbstractFragment {
    Q_OBJECT
public:
    CreatingFragment();
    ~CreatingFragment();

public slots:
    void onBackPressed();
    void onStartPressed();
    //void onCancelPressed();
    //void onDisconnectPressed();
    void onGameStarts();
    void onDrawPlayer(int id);
    void onDeletePlayer(int id);

private:
    QPushButton *backButton;
    QLabel *roomID;
    QLabel *playersCounter;
    int playersCount;
    QPushButton *startButton;
    //QPushButton *cancelButton
};


#endif // CREATINGFRAGMENT_H
