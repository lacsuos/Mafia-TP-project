#ifndef SCREENSFACTORY_H
#define SCREENSFACTORY_H

#include "abstract\include\abstractscreensfactory.h"

#include "fragments\include\optionsfragment.h"
#include "fragments\include\gamefragment.h"
#include "fragments\include\mainfragment.h"
#include "fragments\include\waitingfragment.h"
#include "fragments\include\creatingfragment.h"


namespace screens {
   /*enum Tag{
        kOptions,
        kGame,
        kMain,
        kWaiting,
        kCreating

    };*/
    //obsolete:
    static const QString OPTIONS_TAG = "options";
    static const QString GAME_TAG = "game";
    static const QString MAIN_TAG = "main";
    static const QString WAITING_TAG = "waiting";
    static const QString CREATING_TAG = "creating";
    //static const QString MAIN_TAG = "main";

}

class ScreensFactory : public AbstractScreensFactory {
public:
    ScreensFactory();
    ~ScreensFactory();

    AbstractFragment* create(QString tag) override;
    QString createStart() override;
};

#endif // SCREENSFACTORY_H
