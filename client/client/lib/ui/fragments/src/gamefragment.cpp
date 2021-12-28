#include "gamefragment.h"

#include <utility>
#include "screensfactory.h"

using namespace screens;

const int MAX_PLAYERS = 4;

GameFragment::GameFragment() : ticks(0), isAdminShown(false), state(true),
                               redraw(true) {
    mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerContainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QHBoxLayout *loadingButtonContainer = new QHBoxLayout;
    grid = new QHBoxLayout;
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
    passButton->setStyleSheet("color:grey;font-size:24px");
    //passButton->setEnabled(false);

    buttonContainer->addWidget(passButton);
    loadingButtonContainer->addWidget(passButton);


    stateLabel = new QLabel("Начало игры");
    //stateLabel->setStyleSheet("{ color : grey; }");

    passButton->hide();
    roleLabel = new QLabel("РОЛЬ");

    startMainLayout->addWidget(stateLabel);
    startMainLayout->addWidget(roleLabel);
    startMainLayout->addLayout(grid);
    buttonContainer->addLayout(loadingButtonContainer);

    startMainLayout->addLayout(buttonContainer);
    startContent->setContentsMargins(46, 46, 46, 46);

    centerContainer->setStyleSheet("color:#242424;font-size:24px");
    centerContainer->setLayout(startContent);

    backContainer->setAlignment(Qt::AlignTop);
    startContent->addLayout(backContainer);

    startContent->addLayout(startMainLayout);

    mainHLayout->addWidget(centerContainer);
    mainHLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(startMainLayout);
    this->setLayout(mainVLayout);

    connect(voteButton, &QPushButton::clicked, this, &GameFragment::onVotePressed);
    connect(passButton, &QPushButton::clicked, this, &GameFragment::onPassPressed);

    state = true;
    Player defaultPlayer;
    defaultPlayer.id = 0;
    defaultPlayer.is_live = true;
    defaultPlayer.ip = "0.0.0.0";
    for (int i = 0; i < MAX_PLAYERS; i++) {
        defaultPlayer.id = i + 1;
        players.push_back(defaultPlayer);
        addPlayer(defaultPlayer, this);
    }
}


GameFragment::~GameFragment() {
    while (playerBoxes.count())
        delete playerBoxes.takeLast();
    delete voteButton;
    delete stateLabel;
    delete roleLabel;
    delete backButton;
    delete grid;
}

void GameFragment::onBackPressed() {
    back();
}

void GameFragment::setDayState() {
    state = true;
    stateLabel->setText("День");
    //stateLabel->setStyleSheet("{ color : blue; }");
    //this->setStyleSheet("background_color:#F0F8FF;");
}


void GameFragment::setNightState() {
    state = false;
    stateLabel->setText("Ночь");
    //stateLabel->setStyleSheet("{ color : black; }");
    //this->setStyleSheet("background_color:#A8B6AF");
}

void GameFragment::updateState() {
    if (PlayerData::isDay) {
        setDayState();
    } else {
        setNightState();
    }
}


void GameFragment::setCustomState(const QString &_state) {
    stateLabel->setText(_state);
}

void GameFragment::onVotePressed() {
    bool ok = false;
    int id = QInputDialog::getInt(this, "Vote", "Enter player's ID:", 0, 0, 2147483647, 1, &ok);
    if (ok) {
        onVote(id);
    }
}

void GameFragment::onVote(int id) {
    qDebug() << QString("voted for %1").arg(id);
    if (PlayerData::isDay) {
        Client->vote(id);
    } else if (PlayerData::role == 2) {
        Client->voteMafia(id);
    }
}

void GameFragment::onPassPressed() {
    qDebug() << QString("passed");
//    if (PlayerData::role == 777) {
    if (PlayerData::isDay) {
        Client->nigth();
    } else {
        Client->day();
    }
//    }
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

void GameFragment::update() {
    qDebug() << QString("updated: %1").arg(ticks);
    updateState();
    updatePlayers(savedPlayers);
    roleLabel->setText(QString::number(PlayerData::role));

    if (PlayerData::role == 777 && !isAdminShown) {
        isAdminShown = true;
//        connect(voteButton, &QPushButton::clicked, this, &GameFragment::onVotePressed);

        passButton->show();
    } else {

    }
    players = savedPlayers;
    ticks++;
}

void GameFragment::updatePlayers(const std::vector<Player> &_players) {
//    if (players.size() > _players.size()) {
//        redraw = true;
//    }
//    if (redraw) {
//        delete grid;
//        grid = new QHBoxLayout;
//        PlayerBox* temp;
//        while (playerBoxes.count())
//            delete playerBoxes.takeLast();
//    mainVLayout->addLayout(grid);
//        for (const auto &player: players) {
//            temp = addPlayer(player, this);
//            temp->Show();
//        }


//        redraw = false;
//    }
//    for (int i = 0; i < players.size(); i++) {
//
//    }


/*
    for(int i = 0; i < MAX_PLAYERS; i++) {
        playerBoxes[i]->update(_players[i]);
    }
*/
}

PlayerBox *GameFragment::addPlayer(Player _player, QWidget *parent) {
    PlayerBox *box = new PlayerBox(parent, std::move(_player));
    grid->addWidget(box);
    playerBoxes.append(box);
    connect(box, &PlayerBox::vote, this, &GameFragment::onVote);
    return box;

}

void GameFragment::onLeave() {
    QMessageBox msgBox;
    msgBox.setText("A player has left. Interrupting session.");
    msgBox.exec();
    QApplication::exit();
}

#include "moc_gamefragment.cpp"
