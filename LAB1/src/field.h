#ifndef FIELD_H
#define FIELD_H

#include "cell.h"
#include "common.h"
#include <vector>

// Класс Поля. Владеет памятью клеток и юнитов на них.
class Field {
 public:
    // Конструктор с валидацией размеров
    Field(int width, int height);

    // --- Rule of Five (Управление ресурсами) ---
    ~Field();
    Field(const Field& other);                // Копирование
    Field(Field&& other) noexcept;            // Перемещение
    Field& operator=(const Field& other);     // Присваивание копированием
    Field& operator=(Field&& other) noexcept; // Присваивание перемещением
    // ------------------------------------------

    // Разместить юнита (для инициализации)
    bool PlaceUnit(Unit* unit, int x, int y);

    // Переместить юнита (возвращает true, если действие выполнено: ход или атака)
    bool MoveUnit(int from_x, int from_y, int to_x, int to_y);

    // Получить юнита по координатам (для отрисовки и логики)
    Unit* GetUnitAt(int x, int y) const;

    // Удалить мертвого юнита и очистить память
    void RemoveDeadUnit(int x, int y);
    
    // Нанесение урона конкретной клетке
    void ApplyDamageAt(int x, int y, int damage);

    // Нанесение урона по площади (2x2)
    void ApplyAreaDamage(int center_x, int center_y, int damage);

    // Геттеры размеров
    int GetWidth() const;
    int GetHeight() const;

    // Получить список координат всех врагов (нужно для AI)
    std::vector<Position> GetEnemyPositions() const;

 private:
    int width_;
    int height_;
    Cell** cells_; // Двумерный динамический массив

    // Вспомогательные методы для управления памятью
    void AllocateCells(int w, int h);
    void FreeCells();
    void DeepCopyFrom(const Field& other);
};

#endif  // FIELD_H