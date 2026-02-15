#include "game.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.Run();
    } catch (const std::exception& e) {
        // Глобальный перехват любых критических ошибок
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}