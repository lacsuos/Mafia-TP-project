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

    statusLabel = new QLabel("-");

    buttonContainer->addWidget(statusLabel);
    loadingButtonContainer->addWidget(statusLabel);

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
    delete createButton;
    delete statusLabel;
}
void MainFragment::onCreatePressed() {
    statusLabel->setText("Trying to create");
    statusLabel->setStyleSheet("color:blue");
    Client->createGame();
}
void MainFragment::onPlayPressed() {

    bool ok = false;
    int id = QInputDialog::getInt(this, "Подключение к игре", "Введите ID игры",0,0, 2147483647, 1, &ok);
    if (ok) {
        Client->joinGame(id);
        statusLabel->setText("Trying to join");
        statusLabel->setStyleSheet("color:blue");
        //navigateTo(WAITING_TAG);

    }
}

void MainFragment::onOptionsPressed() {
    navigateTo(OPTIONS_TAG);
}

void MainFragment::onNetError() {
    statusLabel->setText("An network error has happened");
    statusLabel->setStyleSheet("color:red");
}

void MainFragment::onJoined() {
    statusLabel->setText("Succeed");
    statusLabel->setStyleSheet("color:green");
    navigateTo(WAITING_TAG);
}

void MainFragment::onCreated() {
    qDebug()<<"created resolved";
    statusLabel->setText("Succeed");
    statusLabel->setStyleSheet("color:green");
    navigateTo(CREATING_TAG);
}

#include "moc_mainfragment.cpp"
