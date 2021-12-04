#include <iostream>
#include "GameRoom.h"

int main() {
    GameRoom room; // Получаем инф-ю от сервера, заполняем все поля игроков
    room.PlayersInfo();
    for (int i = 0; i < 10; ++i) {
        std::cout << "день: " << i << std::endl;

        std::cout << room.day() << std::endl;
        if (room.day())
            break;
        std::cout << room.evening() << std::endl;
        if (room.evening())
            break;
        room.night();
    }
    return 0;
}

//while (query >> m_id) vector.push(m_id); дальше рассчитываешь сходятся ли голоса и убиваешь игрока посылая m_id >> query
