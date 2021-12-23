#ifndef OPTIONSFRAGMENT_H
#define OPTIONSFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"
#include <QPushButton>

class OptionsFragment: public AbstractFragment {
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
