#include "gamefragment.h"
#include "screensfactory.h"
using namespace screens;

GameFragment::GameFragment() {
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
    connect(BackButton, &QPushButton::clicked, this, &GameFragment::onBackPressed);

    buttonContainer->addWidget(BackButton);
    loadingButtonContainer->addWidget(BackButton);

    StateLabel = new QLabel("Начало игры");
    StateLabel->setStyleSheet("QLabel { color : grey; }");

    startMainLayout->addWidget(StateLabel);
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


GameFragment::~GameFragment() {
    delete BackButton;
}

void GameFragment::onBackPressed() {
    back();
}

void GameFragment::setDayState() {
    StateLabel->setText("День");
    StateLabel->setStyleSheet("QLabel { color : blue; }");
}


void GameFragment::setNightState() {
    StateLabel->setText("Ночь");
    StateLabel->setStyleSheet("QLabel { color : black; }");
}


void GameFragment::setCustomState(const QString state) {
    StateLabel->setText(state);
}

#include "moc_gamefragment.cpp"
