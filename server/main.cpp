#include "server.hpp"

int main() {
    std::string_view SERVER_IP = "127.0.0.1";  // for local game
//    std::string_view SERVER_IP = "0.0.0.0";  // for public game

    net::Server server(SERVER_IP);
    server.run();
    return 0;
}
