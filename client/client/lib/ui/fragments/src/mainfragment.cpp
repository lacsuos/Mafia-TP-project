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

    CreateButton = new QPushButton("Create room");
    CreateButton->setStyleSheet("color:#242424;font-size:24px");
    connect(CreateButton, &QPushButton::clicked, this, &MainFragment::onCreatePressed);

    PlayButton = new QPushButton("Play");
    PlayButton->setStyleSheet("color:#242424;font-size:24px");
    connect(PlayButton, &QPushButton::clicked, this, &MainFragment::onPlayPressed);

    OptionsButton = new QPushButton("Settings");
    OptionsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(OptionsButton, &QPushButton::clicked, this, &MainFragment::onOptionsPressed);

    buttonContainer->addWidget(CreateButton);
    loadingButtonContainer->addWidget(CreateButton);

    buttonContainer->addWidget(PlayButton);
    loadingButtonContainer->addWidget(PlayButton);

    buttonContainer->addWidget(OptionsButton);
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
void MainFragment::onCreatePressed() {
    navigateTo(CREATING_TAG);
}
void MainFragment::onPlayPressed() {

    bool ok = false;
//    int id = QInputDialog::getInt(this, "Подключение к игре", "Введите ID игры",0,0, 2147483647, 1, &ok);
    if (ok/* && ...*/) {
        //connect
        navigateTo(WAITING_TAG);
    }

}

void MainFragment::onOptionsPressed() {
    navigateTo(OPTIONS_TAG);
}


