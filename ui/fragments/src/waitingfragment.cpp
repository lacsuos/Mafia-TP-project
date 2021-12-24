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
    back();
}

//void WaitingFragment::onContinuePressed() {
//    navigateTo(GAME_TAG);
//}

#include "moc_waitingfragment.cpp"
