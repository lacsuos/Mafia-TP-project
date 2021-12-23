//#include <thread>
//#include <memory>
//#include "client.h"
//
//int main(int arc, char* argv[]){
//    std::unique_ptr<std::thread> clientThread;
//    std::unique_ptr<net::Client> client;
//    client = std::make_unique<net::Client>();
//    client->connect();
//    clientThread = std::make_unique<std::thread>([&] { client->run(); });
//    std::cout << "begin accepting\n" << std::endl;
//    while (client->isConnected()){
//        std::cout << client->getLastMsg() << std::endl;
//    }
//    return 0;
//}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
