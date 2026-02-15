#ifndef GAME_H
#define GAME_H

#include "field.h"
#include <string>

class Game {
 public:
    Game();
    ~Game();

    // Главный метод, запускающий игру
    void Run();

 private:
    Field* field_;
    Position player_pos_; // Кэшируем позицию игрока для удобства
    bool is_running_;

    // Инициализация новой игры (создание карты, врагов, игрока)
    void StartNewGame();

    // Сохранение и загрузка
    void SaveGame(const std::string& filename);
    void LoadGame(const std::string& filename);

    // Отрисовка поля в консоли
    void Render();

    // Обработка ввода пользователя
    void ProcessInput();

    // Ход врагов (ИИ)
    void UpdateEnemies();

    // Проверка условий поражения
    void CheckGameState();
};

#endif  // GAME_H