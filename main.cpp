#include <iostream>
#include "PlayRoom.h"

int main() {
    std::vector<int> vecOfId = {1,2,3,4};
    PlayRoom room(vecOfId); // Получаем инф-ю от сервера, заполняем все поля игроков
    std::vector<Player*> plr = room.GetPlayers();
    for (size_t i = 0; i < plr.size(); ++i) {
        std::cout << "Player " << i << std::endl;
        std::cout << "plr[i]->getRoomId() " << plr[i]->getRoomId() << std::endl;
        std::cout << "plr[i]->getAlive() " << plr[i]->getAlive() << std::endl;
        std::cout << "plr[i]->getGlobalId() " << plr[i]->getGlobalId() << std::endl;
        std::cout << "plr[i]->getRole() " << plr[i]->getRole() << std::endl;
        std::cout << "plr[i]->getIsSleep() " << plr[i]->getIsSleep() << std::endl;
    }
    return 0;
}
