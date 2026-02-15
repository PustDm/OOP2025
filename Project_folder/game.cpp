#include "game.h"
#include "player.h"
#include "enemy.h"
#include "spells.h"
#include "common.h"

#include <iostream>
#include <fstream>
#include <conio.h> // Специфично для Windows: _getch() для ввода без Enter
#include <ctime>   // Для случайных чисел (std::time)
#include <cstdlib> // Для std::rand, std::srand

// Вспомогательная функция для генерации случайного заклинания (Часть 2)
std::shared_ptr<ISpell> GenerateRandomSpell() {
    if (std::rand() % 2 == 0) {
        return std::make_shared<FireballSpell>();
    } else {
        return std::make_shared<EarthquakeSpell>();
    }
}

Game::Game() : field_(nullptr), is_running_(true), player_pos_{0, 0} {
    // Инициализация генератора случайных чисел при запуске игры
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Game::~Game() {
    if (field_) delete field_;
}

void Game::Run() {
    std::cout << "=== RPG GAME ===\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "> ";
    
    char choice = _getch(); // Считываем нажатие
    std::cout << choice << "\n";

    try {
        if (choice == '2') {
            std::cout << "Enter filename: ";
            std::string fname;
            std::cin >> fname;
            LoadGame(fname);
        } else {
            StartNewGame();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << ". Starting new game instead.\n";
        system("pause");
        StartNewGame();
    }

    // Основной игровой цикл
    while (is_running_) {
        system("cls"); // Очистка консоли (Windows)
        Render();
        ProcessInput();
        
        if (!is_running_) break;

        UpdateEnemies();
        CheckGameState();
    }
}

void Game::StartNewGame() {
    if (field_) delete field_;
    // Создаем поле 15x15 (Требование: от 10 до 25)
    field_ = new Field(15, 15);

    // Создаем игрока: 100 HP, 10 Dmg, 5 слотов под заклинания
    Player* p = new Player(100, 10, 5);
    
    // Часть 2: Изначально рука содержит только одно случайное заклинание
    p->GetSpellHand().AddSpell(GenerateRandomSpell());

    // Размещаем игрока
    field_->PlaceUnit(p, 0, 0);
    player_pos_ = {0, 0};

    // Размещаем врагов
    field_->PlaceUnit(new Enemy(40, 5), 5, 5);
    field_->PlaceUnit(new Enemy(40, 5), 10, 10);
    field_->PlaceUnit(new Enemy(60, 8), 7, 12);
}

void Game::Render() {
    // Отрисовка карты
    for (int y = 0; y < field_->GetHeight(); ++y) {
        for (int x = 0; x < field_->GetWidth(); ++x) {
            Unit* u = field_->GetUnitAt(x, y);
            if (!u) {
                std::cout << ". ";
            } else if (dynamic_cast<Player*>(u)) {
                std::cout << "P ";
            } else if (dynamic_cast<Enemy*>(u)) {
                std::cout << "E ";
            } else {
                std::cout << "? ";
            }
        }
        std::cout << "\n";
    }

    // Информация об игроке
    Unit* u = field_->GetUnitAt(player_pos_.x, player_pos_.y);
    if (auto p = dynamic_cast<Player*>(u)) {
        std::cout << "\nPlayer HP: " << p->GetHealth() 
                  << " | Score: " << p->GetScore()
                  << " | Spells: " << p->GetSpellHand().GetCount() << "\n";
    }
}

void Game::ProcessInput() {
    std::cout << "Controls: [WASD] Move | [C] Cast | [B] Buy Spell (100pts) | [K] Save | [Q] Quit\n";
    
    char cmd = _getch(); // Чтение клавиши без ожидания Enter
    
    int dx = 0, dy = 0;
    if (cmd == 'w' || cmd == 'W') dy = -1;
    if (cmd == 's' || cmd == 'S') dy = 1;
    if (cmd == 'a' || cmd == 'A') dx = -1;
    if (cmd == 'd' || cmd == 'D') dx = 1;

    // Перемещение
    if (dx != 0 || dy != 0) {
        int nx = player_pos_.x + dx;
        int ny = player_pos_.y + dy;
        
        // Пытаемся пойти или атаковать
        if (field_->MoveUnit(player_pos_.x, player_pos_.y, nx, ny)) {
            // Если перемещение успешно (клетка свободна)
            Unit* u_at_target = field_->GetUnitAt(nx, ny);
            if (u_at_target && dynamic_cast<Player*>(u_at_target)) {
                 player_pos_ = {nx, ny};
            } 
            // Если перемещения не было, значит была АТАКА (клетка занята врагом)
            else {
                // Начисляем очки за удар по врагу (чтобы было на что покупать заклинания)
                Unit* pUnit = field_->GetUnitAt(player_pos_.x, player_pos_.y);
                if (auto p = dynamic_cast<Player*>(pUnit)) {
                    p->AddScore(10); 
                }
            }
        }
    }

    // Часть 2: Реализовать возможность получать новые заклинания (тратить очки)
    if (cmd == 'b' || cmd == 'B') {
        Unit* u = field_->GetUnitAt(player_pos_.x, player_pos_.y);
        if (auto p = dynamic_cast<Player*>(u)) {
            if (p->GetScore() >= 100) {
                if (p->GetSpellHand().AddSpell(GenerateRandomSpell())) {
                    p->AddScore(-100);
                    std::cout << "Bought new spell!\n";
                    system("pause");
                } else {
                    std::cout << "Hand is full!\n";
                    system("pause");
                }
            } else {
                std::cout << "Not enough score (Need 100)!\n";
                system("pause");
            }
        }
    }

    // Использование магии
    if (cmd == 'c' || cmd == 'C') {
        Player* p = dynamic_cast<Player*>(field_->GetUnitAt(player_pos_.x, player_pos_.y));
        if (p && p->GetSpellHand().GetCount() > 0) {
            std::cout << "Direction for spell (WASD): ";
            char d = _getch();
            int sx = player_pos_.x;
            int sy = player_pos_.y;
            
            if (d == 'w') sy--; if (d == 's') sy++; 
            if (d == 'a') sx--; if (d == 'd') sx++;
            
            // Берем первое заклинание и используем
            auto spell = p->GetSpellHand().GetSpell(0);
            if (spell) {
                if (spell->Cast(sx, sy, *field_)) {
                    std::cout << "Spell cast successfully!\n";
                    p->GetSpellHand().RemoveSpell(0); // Удаляем после использования
                    system("pause"); 
                } else {
                    std::cout << "Spell failed (no target/invalid target?)\n";
                    system("pause");
                }
            }
        } else {
            std::cout << "No spells!\n";
            system("pause");
        }
    }

    // Сохранение
    if (cmd == 'k' || cmd == 'K') {
        std::cout << "Enter save filename: ";
        std::string name;
        std::cin >> name;
        try {
            SaveGame(name);
            std::cout << "Game saved!\n";
            system("pause");
        } catch (const std::exception& e) {
            std::cout << "Save failed: " << e.what() << "\n";
            system("pause");
        }
    }

    // Выход
    if (cmd == 'q' || cmd == 'Q') {
        is_running_ = false;
    }
}

void Game::UpdateEnemies() {
    auto enemies = field_->GetEnemyPositions();
    for (const auto& epos : enemies) {
        // Простой ИИ: двигаться к координатам игрока
        int dx = 0, dy = 0;
        if (player_pos_.x > epos.x) dx = 1;
        else if (player_pos_.x < epos.x) dx = -1;
        
        if (player_pos_.y > epos.y) dy = 1;
        else if (player_pos_.y < epos.y) dy = -1;
        
        // Сначала пробуем по горизонтали, потом по вертикали
        if (dx != 0 && field_->MoveUnit(epos.x, epos.y, epos.x + dx, epos.y)) continue;
        if (dy != 0 && field_->MoveUnit(epos.x, epos.y, epos.x, epos.y + dy)) continue;
    }
}

void Game::CheckGameState() {
    Unit* u = field_->GetUnitAt(player_pos_.x, player_pos_.y);
    // Если игрока нет на его позиции или он мертв
    if (!u || !dynamic_cast<Player*>(u) || !u->IsAlive()) {
        system("cls");
        std::cout << "\n\n!!! GAME OVER !!!\n\n";
        is_running_ = false;
        system("pause");
    }
}

void Game::SaveGame(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) throw GameException("Cannot open file for writing");
    
    // Формат: Ширина Высота
    out << field_->GetWidth() << " " << field_->GetHeight() << "\n";
    
    // Далее проходим по всем клеткам
    for (int y = 0; y < field_->GetHeight(); ++y) {
        for (int x = 0; x < field_->GetWidth(); ++x) {
            Unit* u = field_->GetUnitAt(x, y);
            if (dynamic_cast<Player*>(u)) {
                // P здоровье урон ОЧКИ (важно сохранять очки)
                auto p = dynamic_cast<Player*>(u);
                out << "P " << p->GetHealth() << " " << p->GetDamage() << " " << p->GetScore() << " ";
            } else if (dynamic_cast<Enemy*>(u)) {
                // E здоровье урон заглушка_очков(0)
                out << "E " << u->GetHealth() << " " << u->GetDamage() << " 0 ";
            } else {
                // . (пусто) 0 0 0
                out << ". 0 0 0 ";
            }
        }
        out << "\n"; // Новая строка для удобства чтения
    }
}

void Game::LoadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw GameException("Cannot open file for reading");
    
    int w, h;
    in >> w >> h;
    
    if (in.fail()) throw GameException("File format error (header)");

    if (field_) delete field_;
    field_ = new Field(w, h); 
    
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            char type;
            int hp, dmg, score;
            // Считываем 4 параметра, так как в SaveGame мы пишем 4 значения для консистентности
            in >> type >> hp >> dmg >> score;
            
            if (in.fail()) throw GameException("File format error (data)");

            if (type == 'P') {
                Player* p = new Player(hp, dmg, 5);
                p->AddScore(score); // Восстанавливаем очки
                // При загрузке даем 1 случайное заклинание, если сохранение не поддерживает хранение списка
                p->GetSpellHand().AddSpell(GenerateRandomSpell());
                field_->PlaceUnit(p, x, y);
                player_pos_ = {x, y};
            } else if (type == 'E') {
                field_->PlaceUnit(new Enemy(hp, dmg), x, y);
            }
        }
    }
}