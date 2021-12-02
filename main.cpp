#include <iostream>
#include "GameRoom.h"

int main() {
    GameRoom room; // Получаем инф-ю от сервера, заполняем все поля игроков
    /*
     * Возвращаем информацию серверу
     * Ждем, когда ведущий нажмет на кнопку "начать день"
     */
    room.GameLoop(); // запуск игры
    return 0;
}

//while (query >> m_id) vector.push(m_id); дальше рассчитываешь сходятся ли голоса и убиваешь игрока посылая m_id >> query
