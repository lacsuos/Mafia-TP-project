#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "abstract\include\abstractfragment.h"

#include <QPushButton>

class GameFragment: public AbstractFragment {
    Q_OBJECT
public:
    GameFragment();
    ~GameFragment();

public slots:
    void onBackPressed();

private:
    QPushButton *BackButton;
};

#endif // SETTINGSFRAGMENT_H
