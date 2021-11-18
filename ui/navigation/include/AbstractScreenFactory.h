#pragma once

#include <QString>
#include "AbstractFragment.h">

namespace screen{
    const QString MENU = "menu";
    const QString PLAY = "play";
}

class AbstractScreenFactory{
public:
    AbstractScreenFactory();
    ~AbstractScreenFactory();

    AbstractFragment* createFragment(QString flag);
    QString createStart();
};