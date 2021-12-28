#include "waitingfragment.h"
#include "screensfactory.h"
using namespace screens;

WaitingFragment::WaitingFragment() {
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
    connect(backButton, &QPushButton::clicked, this, &WaitingFragment::onBackPressed);

    /*
    continueButton = new QPushButton("Continue");
    continueButton->setStyleSheet("color:#242424;font-size:24px");
    connect(continueButton, &QPushButton::clicked, this, &WaitingFragment::onContinuePressed);
    */
    roomID = new QLabel(QString("Room ID: ") + QString::number(PlayerData::roomID));

    buttonContainer->addWidget(roomID);

    playersCounter = new QLabel("");

    buttonContainer->addWidget(playersCounter);

    buttonContainer->addWidget(backButton);
    loadingButtonContainer->addWidget(backButton);

//    buttonContainer->addWidget(continueButton);
//    loadingButtonContainer->addWidget(continueButton);

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


WaitingFragment::~WaitingFragment() {
    delete backButton;

}

void WaitingFragment::onBackPressed() {
    Client->disconnect();
    back();
}

//void WaitingFragment::onContinuePressed() {
//    navigateTo(GAME_TAG);
//}

void WaitingFragment::onGameStarts() {
    navigateTo(GAME_TAG);
    QMessageBox msgBox;
    msgBox.setText("Game starts!");
    msgBox.exec();
}

void WaitingFragment::onDeletePlayer(int id) {


}

void WaitingFragment::onDrawPlayer(int id) {
    playersStr = playersStr + QString::number(id) + QString(", ");
    playersCounter->setText(playersStr);
}

#include "moc_waitingfragment.cpp"
