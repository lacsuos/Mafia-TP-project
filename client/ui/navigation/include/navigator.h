#pragma once

#include <QStackedWidget>
#include <QObject>
#include <QLinkedList>
#include "AbstractFragment.h"
#include "AbstractScreenFactory.h"

class Navigator:public QObject{
    Q_OBJECT
private:
    QStackedWidget* current;
    AbstractScreenFactory* screenFactory;
    QLinkiedList<AbstractFragment*> = stack;

    AbstractFragment* createAndConnect(QString flag);
    void connectFragment(AbstractFragment *fragment);
    void disconnectFragment(AbstractFragment *fragment);

public:
    Navigator(QStackedWidget* container, AbstractScreenFactory* _screenFactory);
    ~Navigator();
    AbstractFragment* createStart();

public slots:
    void back();
    void moveTo(QString flag);
    void newRootScreem(QString flag);
    void replace(QString flag);
};