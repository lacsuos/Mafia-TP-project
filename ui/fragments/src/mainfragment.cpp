#include "mainfragment.h"
#include "screensfactory.h"
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

    createButton = new QPushButton("Create room");
    createButton->setStyleSheet("color:#242424;font-size:24px");
    connect(createButton, &QPushButton::clicked, this, &MainFragment::onCreatePressed);

    playButton = new QPushButton("Play");
    playButton->setStyleSheet("color:#242424;font-size:24px");
    connect(playButton, &QPushButton::clicked, this, &MainFragment::onPlayPressed);

    optionsButton = new QPushButton("Settings");
    optionsButton->setStyleSheet("color:#242424;font-size:24px");
    connect(optionsButton, &QPushButton::clicked, this, &MainFragment::onOptionsPressed);

    buttonContainer->addWidget(createButton);
    loadingButtonContainer->addWidget(createButton);

    buttonContainer->addWidget(playButton);
    loadingButtonContainer->addWidget(playButton);

    buttonContainer->addWidget(optionsButton);
    loadingButtonContainer->addWidget(optionsButton);

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
    delete playButton;
    delete optionsButton;
}
void MainFragment::onCreatePressed() {
    navigateTo(CREATING_TAG);
}
void MainFragment::onPlayPressed() {

    bool ok = false;
    int id = QInputDialog::getInt(this, "Подключение к игре", "Введите ID игры",0,0, 2147483647, 1, &ok);
    if (ok/* && ...*/) {
        //connect
        navigateTo(WAITING_TAG);
    }

}

void MainFragment::onOptionsPressed() {
    navigateTo(OPTIONS_TAG);
}

#include "moc_mainfragment.cpp"
