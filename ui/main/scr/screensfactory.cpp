#include "main/include/screensfactory.h"


using namespace screens;

ScreensFactory::ScreensFactory() {

}

ScreensFactory::~ScreensFactory() {

}

AbstractFragment* ScreensFactory::create(QString tag) {
    qDebug("ScreensFactory create");

    if (tag == OPTIONS_TAG) {
        return new OptionsFragment;
    }
    if (tag == GAME_TAG) {
        return new GameFragment;
    }
    if (tag == MAIN_TAG) {
        return new MainFragment;
    }
    return nullptr;
}

QString ScreensFactory::createStart() {
    return MAIN_TAG;
}
