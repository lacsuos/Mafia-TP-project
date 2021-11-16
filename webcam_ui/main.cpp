#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QNetworkRequest>
#include <QMediaPlayer> 
#include <QVideoWidget>
#include <QDebug>
#include <iostream>

class Widget : public QWidget
{
public:
    Widget(QWidget *parent = 0)
        : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        QMediaPlayer *player = new QMediaPlayer(this);
        QVideoWidget *vw = new QVideoWidget;
        QUrl url = QUrl("gst-pipeline: videotestsrc ! xvimagesink name=\"qtvideosink\"");
        //QNetworkRequest rtspRequest(url);
        layout->addWidget(vw);
        player->setVideoOutput(vw);
        player->setMedia(QUrl("gst-pipeline: videotestsrc ! xvimagesink name=\"qtvideosink\""));
        player->play();
        qDebug() << player->state();
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Widget widget;
    widget.resize(400, 300);
    widget.show();

    return app.exec();
}