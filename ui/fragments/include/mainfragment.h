#ifndef MAINFRAGMENT_H
#define MAINFRAGMENT_H

#include "abstractfragment.h"
#include "screensfactory.h"

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
    QPushButton *createButton;
    QPushButton *playButton;
    QPushButton *optionsButton;
};

#endif // MAINFRAGMENT_H
