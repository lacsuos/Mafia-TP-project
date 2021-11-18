#pragma once

#include "AbstractFragment.h"
#include "webcam.h"
#include <vector>
#include <QWidget>
#include <QVBoxLayout>
#include <QNetworkRequest>
#include <QMediaPlayer> 
#include <QVideoWidget>

template <typedef T>
class GameFragment : public AbstractFragment{
    Q_OBJECT
private:
    std::vector<T> players;
    Broadcaster* webcamServer;
    QBVboxLayout** layout;
    QVideoWidget** vw;
    QMediaPlayer** mediaPlayer;

    void showVideo();
    void StopVideo();

private slots:
    void onClickedVote();

public: 
    GameFragment();
    ~GameFragment();

public slots:
    void changePlayerStatus(std::vector<T> players);
    void chagneGameStatus();
};