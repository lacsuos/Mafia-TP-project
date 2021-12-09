#include "mainfragment.h"

using namespace screens;

MainFragment::MainFragment() {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerContainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QHBoxLayout *loadingButtonContainer = new QHBoxLayout;


    PlayButton = new QPushButton("Play");
    PlayButton->setStyleSheet("color:#242424;font-size:24px");
    connect(PlayButton, &QPushButton::clicked, this, &MainFragment::onPlayPressed);

    OptionsButton = new QPushButton("Settings");
    OptionsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(OptionsButton, &QPushButton::clicked, this, &MainFragment::onOptionsPressed);

    buttonContainer->addWidget(PlayButton);
    buttonContainer->addWidget(OptionsButton);
    loadingButtonContainer->addWidget(PlayButton);
    loadingButtonContainer->addWidget(OptionsButton);

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

MainFragment::~MainFragment() {
    delete PlayButton;
    delete OptionsButton;
}

void MainFragment::onPlayPressed() {
    navigateTo(GAME_TAG);
}

void MainFragment::onOptionsPressed() {
    navigateTo(OPTIONS_TAG);
}

