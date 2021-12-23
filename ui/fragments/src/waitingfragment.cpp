#include "waitingfragment.h"

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


    BackButton = new QPushButton("Back");
    BackButton->setStyleSheet("color:#242424;font-size:24px");
    connect(BackButton, &QPushButton::clicked, this, &WaitingFragment::onBackPressed);

    ContinueButton = new QPushButton("Continue");
    ContinueButton->setStyleSheet("color:#242424;font-size:24px");
    connect(ContinueButton, &QPushButton::clicked, this, &WaitingFragment::onContinuePressed);

    buttonContainer->addWidget(BackButton);
    loadingButtonContainer->addWidget(BackButton);

    buttonContainer->addWidget(ContinueButton);
    loadingButtonContainer->addWidget(BackButton);

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
    delete BackButton;
}

void WaitingFragment::onBackPressed() {
    back();
}

void WaitingFragment::onContinuePressed() {
    navigateTo(GAME_TAG);
}
