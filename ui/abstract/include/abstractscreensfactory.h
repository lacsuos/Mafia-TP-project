#ifndef ABSTRACTSCREENSFACTORY_H
#define ABSTRACTSCREENSFACTORY_H

#include "abstractfragment.h"

#include <QString>


class AbstractScreensFactory {
public:
    AbstractScreensFactory();
    virtual ~AbstractScreensFactory();

    virtual AbstractFragment* create(QString tag) = 0;
    virtual QString createStart() = 0;
};

#endif // ABSTRACTSCREENSFACTORY_H
