#ifndef CREATINGFRAGMENT_H
#define CREATINGFRAGMENT_H

#include "abstract\include\abstractfragment.h"
#include "main\include\screensfactory.h"

#include <QPushButton>

class CreatingFragment: public AbstractFragment {
    Q_OBJECT
public:
    CreatingFragment();
    ~CreatingFragment();

public slots:
    void onBackPressed();

private:
    QPushButton *BackButton;
};


#endif // CREATINGFRAGMENT_H
