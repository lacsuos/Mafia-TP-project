#include <vector>
#include <QWidget>
#include <QVBoxLayout>
#include <QNetworkRequest>
#include <QMediaPlayer> 
#include <QVideoWidget>

class Player : public QWidget {
private:
    std::string ip;
    int id;
    std::string role;
    bool is_alive;
    QMediaPlayer *player;
    QVideoWidget *vw;
    QVBoxLayout *layout
public:
    Player(std::string _ip, int _id, std::string _role);
    ~Player();
    void playVideo();
    void stopVideo();
    std::string getRole(){ return role; }
    int getID(){ return id; }
    
}