#pragma once

#include <QString>
#include "AbstractScreenFactory.h"

namespace screen{
    const QString MENU = "menu";
    const QString PLAY = "play";
}

class ScreenFactory: public AbstractScreenFactory{
public:
    ScreenFactory();
    ~ScreenFactory();

    AbstractFragment* createFragment(QString flag) override;
    QString createStart() override;
};