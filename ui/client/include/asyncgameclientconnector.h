#ifndef ASYNCGAMECLIENTCONNECTOR_H
#define ASYNCGAMECLIENTCONNECTOR_H

#include<QObject>
#include<QThread>
#include "gameclient.h"
class AsyncGameClientConnector : public QObject
{
    Q_OBJECT
    QThread thread;
    GameClient client;
public:
    AsyncGameClientConnector();
    ~AsyncGameClientConnector();
};

#endif // ASYNCGAMECLIENTCONNECTOR_H
