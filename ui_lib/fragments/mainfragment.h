#ifndef MENUFRAGMENT_H
#define MENUFRAGMENT_H

#include "base\basefragment.h"
#include "screenfactory.h"

#include <QPushButton>

class MainFragment: public BaseFragment {
    Q_OBJECT

public:
    MainFragment();
    ~MainFragment();
public slots:
    void onPlayPressed();
    void onOptionsPressed();

private:
    QPushButton *PlayButton;
    QPushButton *OptionsButton;
};

#endif // MENUFRAGMENT_H
