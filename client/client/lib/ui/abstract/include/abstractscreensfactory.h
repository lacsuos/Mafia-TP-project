#ifndef ABSTRACTSCREENSFACTORY_H
#define ABSTRACTSCREENSFACTORY_H

#include "abstractfragment.h"

#include <QString>

/*namespace screens {
    enum class Tag{
        kOptions,
        kGame,
        kMain,
        kWaiting,
        kCreating

    };
    //obsolete:
    static const QString OPTIONS_TAG = "options";
    static const QString GAME_TAG = "game";
    static const QString MAIN_TAG = "main";
    static const QString WAITING_ROOM_TAG = "options";
    //static const QString GAME_TAG = "game";
    //static const QString MAIN_TAG = "main";

}
*/
class AbstractScreensFactory {
public:
    AbstractScreensFactory();
    virtual ~AbstractScreensFactory();

    virtual AbstractFragment* create(QString tag) = 0;
    virtual QString createStart() = 0;
};

#endif // ABSTRACTSCREENSFACTORY_H
