#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "base\basefragment.h"

#include <QPushButton>

class GameFragment: public BaseFragment {
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
