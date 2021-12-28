#include "playerbox.h"

PlayerBox::PlayerBox(QWidget *parent, Player _player) : QFrame(parent), player(_player)
{
    //Подключение камеры


    this->setFrameShape(Box);
    this->setFrameShadow(Raised);
    QVBoxLayout *container = new QVBoxLayout(this);

    voteButton = new QPushButton(QString("Vote for №%1").arg(player.id));
    voteButton->setStyleSheet("color:#242424;font-size:24px");
    container->addWidget(voteButton);
    connect(voteButton, &QPushButton::clicked, this, &PlayerBox::onVotePressed);

    this->setLayout(container);
    if (!player.is_live) {
        this->setStyleSheet("background_color:=grey;");
    }
}
void PlayerBox::update(Player _player) {
    //Обновление и отключение камеры
     player = _player;
     voteButton->setText(QString("Vote for №%1").arg(player.id));
     qDebug() << QString("updated button for player №%1").arg(player.id);

     if (!player.is_live) {
         this->setStyleSheet("background_color:grey;");
     }
}
PlayerBox::~PlayerBox() {
    delete voteButton;
}
int PlayerBox::getID() {
    return player.id;
}
void PlayerBox::onVotePressed() {
    emit vote(player.id);
}
#include "moc_playerbox.cpp"
