#include "gamefragment.h"
#include "screensfactory.h"
using namespace screens;

GameFragment::GameFragment() :  ticks(0), playerCount(4),  isAdminShown(false), state(true), redraw(true) {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerContainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QHBoxLayout *loadingButtonContainer = new QHBoxLayout;
    grid = new QHBoxLayout(this);
    adminBox = new QVBoxLayout;

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


    //Определить Роли!!!

    passButton = new QPushButton("Pass");
    passButton->setStyleSheet("color:#242424;font-size:24px");

    adminBox->addWidget(passButton);
    passButton->hide();

    stateLabel = new QLabel("Начало игры");
    stateLabel->setStyleSheet("QLabel { color : grey; }");

    roleLabel = new QLabel("РОЛЬ");

    startMainLayout->addWidget(stateLabel);
    startMainLayout->addWidget(roleLabel);
    buttonContainer->addLayout(loadingButtonContainer);
    buttonContainer->addLayout(adminBox);

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
    this->setStyleSheet("background_color:#F0F8FF;");
}


void GameFragment::setNightState() {
    state = false;
    stateLabel->setText("Ночь");
    stateLabel->setStyleSheet("QLagameIterationbel { color : black; }");
    this->setStyleSheet("background_color:#A8B6AF");
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
        onVote(id);
    }
}

void GameFragment::onVote(int id) {
    if (PlayerData::isDay){
        Client->vote(id);
    } else if (PlayerData::role == 2) {
        Client->voteMafia(id);
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
void GameFragment::update(std::vector<Player> _players) {
    qDebug() << QString("updated: %1").arg(ticks);
    updateState();
    updatePlayers(_players);
    roleLabel->setText(QString::number(PlayerData::role));
    if (PlayerData::role == 777 && !isAdminShown) {
        isAdminShown = true;
        connect(voteButton, &QPushButton::clicked, this, &GameFragment::onVotePressed);

        voteButton->show();
    }
    players = _players;
    ticks++;
}

void GameFragment::updatePlayers(std::vector<Player> _players) {
    if (playerCount > _players.size()){
            redraw = true;
    }
    if (redraw) {
        delete grid;
        grid = new QHBoxLayout(this);
        while (playerBoxes.count())
             delete playerBoxes.takeLast();

        for (auto player : players) {
            addPlayer(player, this);
        }
        redraw = false;
        playerCount = _players.size();
        return;
    }
    for (int i = 0; i < playerCount; i++) {

    }
    playerCount = _players.size();

}

void GameFragment::addPlayer(Player _player, QWidget* parent) {
    PlayerBox* box = new PlayerBox(parent, _player);
    grid->addWidget(box);
    playerBoxes.append(box);
    connect(box, &PlayerBox::vote, this, &GameFragment::onVote);

}

#include "moc_gamefragment.cpp"
