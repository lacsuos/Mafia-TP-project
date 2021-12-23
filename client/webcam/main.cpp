#include "webcam.h"

int main(int argc, char* argv[]){
    Broadcaster* server = new Broadcaster;
    server->launchServer();
    delete server;
    return 0;
}