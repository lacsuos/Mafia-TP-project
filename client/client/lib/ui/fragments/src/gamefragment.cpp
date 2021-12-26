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


    backButton = new QPushButton("Back");
    backButton->setStyleSheet("color:#242424;font-size:24px");
    connect(backButton, &QPushButton::clicked, this, &GameFragment::onBackPressed);

    buttonContainer->addWidget(backButton);
    loadingButtonContainer->addWidget(backButton);

    voteButton = new QPushButton("vote");
    voteButton->setStyleSheet("color:#242424;font-size:24px");
    connect(voteButton, &QPushButton::clicked, this, &GameFragment::onVotePressed);

    buttonContainer->addWidget(voteButton);
    loadingButtonContainer->addWidget(voteButton);


    stateLabel = new QLabel("Начало игры");
    stateLabel->setStyleSheet("QLabel { color : grey; }");

    startMainLayout->addWidget(stateLabel);
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

    state = true;
}


GameFragment::~GameFragment() {
    delete backButton;
}

void GameFragment::onBackPressed() {
    back();
}

void GameFragment::setDayState() {
    state = true;
    stateLabel->setText("День");
    stateLabel->setStyleSheet("QLabel { color : blue; }");
}


void GameFragment::setNightState() {
    state = false;
    stateLabel->setText("Ночь");
    stateLabel->setStyleSheet("QLabel { color : black; }");
}


void GameFragment::setCustomState(const QString state) {
    stateLabel->setText(state);
}
void GameFragment::onVotePressed() {
    bool ok = false;
    int id = QInputDialog::getInt(this, "Vote", "Enter player's ID:",0,0, 2147483647, 1, &ok);
    if (ok) {
        //Client->vote(id);


    }
}
#include "moc_gamefragment.cpp"
