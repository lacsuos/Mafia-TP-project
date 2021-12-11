#include <thread>
#include <memory>
#include "client.h"

int main(int arc, char* argv[]){
    std::unique_ptr<std::thread> clientThread;
    std::unique_ptr<std::thread> resolverThread;
    std::unique_ptr<net::Client> client;
    client = std::make_unique<net::Client>();
    client->connect();
    clientThread = std::make_unique<std::thread>([&] { client->run(); });
    clientThread->detach();
    std::cout << "begin accepting\n" << std::endl;
    resolverThread = std::make_unique<std::thread> ([&] { 
        while (client->isConnected()){
            std::cout << client->getLastMsg() << std::endl;
        }
    });
    int choose;
    while (true) {
        std::cin >> choose;
        switch (choose)
        {
        case 1:
            client->createRoom();
            break;
        case 2:
            std::cout << "Enter room id" << std::endl;
            int id;
            std::cin >> id;
            client->joinRoom(id);
            break;
        case 0:
            return 0;
        default:
            break;
        }
    }
    return 0;
}