#include "server.hpp"

int main() {
    constexpr std::string_view SERVER_IP = "127.0.0.1";  // for local game
    //constexpr std::string_view SERVER_IP = "0.0.0.0";  // for public game

    net::Server server(SERVER_IP);
    server.run();
    return 0;
}
