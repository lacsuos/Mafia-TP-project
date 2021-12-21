#ifndef MAINFRAGMENT_H
#define MAINFRAGMENT_H

#include "abstract\include\abstractfragment.h"
#include "main\include\screensfactory.h"

#include <QPushButton>
#include <QInputDialog>

class MainFragment: public AbstractFragment {
    Q_OBJECT

public:
    MainFragment();
    ~MainFragment();
public slots:
    void onCreatePressed();
    void onPlayPressed();
    void onOptionsPressed();

private:
    QPushButton *CreateButton;
    QPushButton *PlayButton;
    QPushButton *OptionsButton;
};

#endif // MAINFRAGMENT_H
