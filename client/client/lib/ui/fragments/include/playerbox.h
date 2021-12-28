#ifndef PLAYERBOX_H
#define PLAYERBOX_H

#include <QFrame>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include "resolver.h"

using namespace resolver;

class PlayerBox : public QFrame
{
    Q_OBJECT
public:

    PlayerBox(QWidget *parent, Player _player);
    ~PlayerBox();
    int getID();
    QPushButton *voteButton;
    resolver::Player player;
    void update(Player _player);
public slots:
    QImage* image;
    void onVotePressed();
signals:
    void vote(int id);

private:

};

#endif // PLAYERBOX_H
