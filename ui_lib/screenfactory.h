#ifndef SCREENFACTORY_H
#define SCREENFACTORY_H

#include "Base\basescreensfactory.h"

#include "Fragments\optionsfragment.h"
#include "Fragments\gamefragment.h"
#include "Fragments\mainfragment.h"



namespace screens {
    static const QString OPTIONS_TAG = "options";
    static const QString GAME_TAG = "game";
    static const QString MAIN_TAG = "main";
}

class ScreensFactory : public BaseScreensFactory {
public:
    ScreensFactory();
    ~ScreensFactory();

    BaseFragment* create(QString tag) override;
    QString createStart() override;
};

#endif // SCREENFACTORY_H
