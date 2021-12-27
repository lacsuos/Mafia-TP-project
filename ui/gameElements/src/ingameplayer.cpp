#include "ingameplayer.h"

Player::Player(std::string _ip, int _id, std::string _role) : ip(_ip), id(_id), role(_role)
{
    QVideoWidget vw = new QVideoWidget;
    QVBoxLayout layout = new QVBoxLayout(this);
    QMediaPlayer player = new QMediaPlayer(this);
    QUrl url = QUrl("rtsp://" + ip + ":8554/webcam");
    QNetworkRequest rtspRequest(url);
    layout->addWidget(vw);
    player->setVideoOutput(vw);
    player->setMedia(rtspRequest);
};
void Player::playVideo(){
    player->play();
}

void Player::stopVideo(){
    player->stop();
}
