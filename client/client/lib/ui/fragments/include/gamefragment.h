#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"
#include "resolver.h"
#include "playerbox.h"

#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>

#include <vector>
#include <QWidget>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
//#include <QGridLayout>
//#include <QNetworkRequest>
//#include <QMediaPlayer>
//#include <QVideoWidget>

using namespace resolver;

class GameFragment : public AbstractFragment {
Q_OBJECT
public:
    GameFragment();

    ~GameFragment();


public slots:

    void onBackPressed();

    //void addPlayers();
    void setDayState();

    void setNightState();

    void setCustomState(const QString& state);

    void onVotePressed();

    void onVote(int id);

    void updateState();

    void onPassPressed();

    void onCitizenWin();

    void onMafiaWin();

    void onWin();

    void onLose();

    void onLeave();

    void update();

private:

//    std::vector<T> players;
//    Broadcaster* webcamServer;
//    QBVboxLayout** layout;
//    QVideoWidget** vw;
//    QMediaPlayer** mediaPlayer;
    QVector<PlayerBox *> playerBoxes;
    std::vector<Player> players;

    int ticks;
    int playerCount;
    bool isAdminShown;
    bool state;
    bool redraw;

    QVBoxLayout *mainVLayout;
    QHBoxLayout *grid;
    QPushButton *backButton;
    QPushButton *voteButton;
    QPushButton *passButton;
    QLabel *stateLabel;
    QLabel *roleLabel;

    void updatePlayers(const std::vector<Player>& players);

    PlayerBox* addPlayer(resolver::Player player, QWidget *parent);
};

#endif // GAMEFRAGMENT_H
