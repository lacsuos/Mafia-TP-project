#include "creatingfragment.h"
#include "screensfactory.h"
using namespace screens;

CreatingFragment::CreatingFragment() {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerContainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QHBoxLayout *loadingButtonContainer = new QHBoxLayout;


    backButton = new QPushButton("Back");
    backButton->setStyleSheet("color:#242424;font-size:24px");
    connect(backButton, &QPushButton::clicked, this, &CreatingFragment::onBackPressed);

    buttonContainer->addWidget(backButton);
    loadingButtonContainer->addWidget(backButton);

    startButton = new QPushButton("Start");
    startButton->setStyleSheet("color:#242424;font-size:24px");
    startButton->setEnabled(false);
    buttonContainer->addWidget(startButton);
    loadingButtonContainer->addWidget(startButton);

    roomID = new QLabel("-");

    buttonContainer->addWidget(roomID);

    playersCounter = new QLabel("0");
    playersCount = 1;

    buttonContainer->addWidget(playersCounter);


    buttonContainer->addLayout(loadingButtonContainer);

    startMainLayout->addLayout(buttonContainer);
    startContent->setContentsMargins(46,46,46,46);

    centerContainer->setStyleSheet("color:#242424;font-size:24px");
    centerContainer->setLayout(startContent);

    backContainer->setAlignment(Qt::AlignTop);
    startContent->addLayout(backContainer);

    startContent->addLayout(startMainLayout);

    mainHLayout->addWidget(centerContainer);
    mainHLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->setAlignment(Qt::AlignCenter);

    this->setLayout(mainVLayout);
}


CreatingFragment::~CreatingFragment() {
    delete roomID;
    delete playersCounter;
    delete backButton;
    //delete cancelButton;
}

void CreatingFragment::onBackPressed() {
    Client->disconnect();
    back();
}

void CreatingFragment::onDeletePlayer(int id) {
    playersCount--;
    playersCounter->setText(QString::number(playersCount));
    if (playersCount < 4) {
        startButton->setEnabled(false);
    }


}

void CreatingFragment::onDrawPlayer(int id) {
    playersCount++;
    playersCounter->setText(QString::number(playersCount));
    if (playersCount >= 4) {
        connect(startButton, &QPushButton::clicked, this, &CreatingFragment::onStartPressed);
        startButton->setEnabled(true);
    }
}

void CreatingFragment::onStartPressed() {
    Client->startGame();
    navigateTo(GAME_TAG);
}

#include "moc_creatingfragment.cpp"
