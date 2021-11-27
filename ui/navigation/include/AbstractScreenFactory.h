#pragma once

#include <QString>
#include "AbstractFragment.h">


class AbstractScreenFactory{
public:
    AbstractScreenFactory();
    ~AbstractScreenFactory();

    virtual AbstractFragment* createFragment(QString flag);
    virtual QString createStart();
};