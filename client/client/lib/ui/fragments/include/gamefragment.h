#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"

#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>

#include <vector>
#include <QWidget>
#include <QVBoxLayout>
#include <QNetworkRequest>
#include <QMediaPlayer>
#include <QVideoWidget>

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
    void onVotePressed();
    void updateState();
    void onPassPressed();

    void onCitisenWin();
    void onMafiaWin();
    void onWin();
    void onLose();

private:

//    std::vector<T> players;
//    Broadcaster* webcamServer;
//    QBVboxLayout** layout;
//    QVideoWidget** vw;
//    QMediaPlayer** mediaPlayer;


    bool state;
    QPushButton *backButton;
    QPushButton *voteButton;
    QPushButton *passButton;
    QLabel *stateLabel;
};

#endif // GAMEFRAGMENT_H
