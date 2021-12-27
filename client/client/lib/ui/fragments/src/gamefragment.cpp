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

    voteButton = new QPushButton("Vote");
    voteButton->setStyleSheet("color:#242424;font-size:24px");
    connect(voteButton, &QPushButton::clicked, this, &GameFragment::onVotePressed);

    buttonContainer->addWidget(voteButton);
    loadingButtonContainer->addWidget(voteButton);

    isMaster = false;
    ticks = 0;

    //Определить Роли!!!

    passButton = new QPushButton("Pass");
    passButton->setStyleSheet("color:#242424;font-size:24px");




    stateLabel = new QLabel("Начало игры");
    stateLabel->setStyleSheet("QLabel { color : grey; }");

    roleLabel = new QLabel("РОЛЬ");

    startMainLayout->addWidget(stateLabel);
    startMainLayout->addWidget(roleLabel);
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
    stateLabel->setStyleSheet("QLagameIterationbel { color : black; }");
}

void GameFragment::updateState() {
    if (PlayerData::isDay){
        setDayState();
    } else {
        setNightState();
    }
}


void GameFragment::setCustomState(const QString _state) {
    stateLabel->setText(_state);
}

void GameFragment::onVotePressed() {
    bool ok = false;
    int id = QInputDialog::getInt(this, "Vote", "Enter player's ID:",0,0, 2147483647, 1, &ok);
    if (ok) {

        if (PlayerData::isDay){
            Client->vote(id);
        } else if (PlayerData::role == 2) {
            Client->voteMafia(id);
        }



    }
}

void GameFragment::onPassPressed() {
    if (PlayerData::role == 777) {
        if (PlayerData::isDay) {
            Client->nigth();
        } else {
            Client->day();
        }
    }
}

void GameFragment::onCitizenWin() {
    QMessageBox msgBox;
    msgBox.setText("The City has defeated Mafia");
    msgBox.exec();
}
void GameFragment::onMafiaWin() {
    QMessageBox msgBox;
    msgBox.setText("The Mafia has overcome the Law");
    msgBox.exec();
}
void GameFragment::onLose() {
    QMessageBox msgBox;
    msgBox.setText("Your side has lost");
    msgBox.exec();
}
void GameFragment::onWin() {
    QMessageBox msgBox;
    msgBox.setText("Your side has won");
    msgBox.exec();
}
void GameFragment::updatePlayers(std::vector<resolver::Player> players) {
    qDebug() << QString("updated: %1").arg(ticks);
    roleLabel->setText(QString::number(PlayerData::role))
    /*if (PlayerData::role == 777 && !isMaster) {
        isMaster = true;
        connect(voteButton, &QPushButton::clicked, this, &GameFragment::onVotePressed);

        buttonContainer->addWidget(voteButton);
        loadingButtonContainer->addWidget(voteButton);
    }*/
    ticks++;
}
#include "moc_gamefragment.cpp"
