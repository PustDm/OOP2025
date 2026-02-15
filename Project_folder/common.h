#ifndef COMMON_H
#define COMMON_H

#include <stdexcept>
#include <string>

// Перечисление направлений движения (для читаемости кода)
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

// Структура координат (используется и в Поле, и в Игре)
struct Position {
    int x;
    int y;

    // Оператор сравнения для удобства
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Пользовательский класс исключений для логики игры
class GameException : public std::runtime_error {
 public:
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif  // COMMON_H