#ifndef OPTIONSFRAGMENT_H
#define OPTIONSFRAGMENT_H

#include "base\basefragment.h"

#include <QPushButton>

class OptionsFragment: public BaseFragment {
    Q_OBJECT
public:
    OptionsFragment();
    ~OptionsFragment();

public slots:
    void onBackPressed();

private:
    QPushButton *BackButton;
};

#endif // SETTINGSFRAGMENT_H
