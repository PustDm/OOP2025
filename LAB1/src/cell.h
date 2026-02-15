#ifndef CELL_H
#define CELL_H

#include "unit.h"

class Cell {
 public:
    Cell();

    // Проверяет, стоит ли кто-то в клетке
    bool IsOccupied() const;

    // Возвращает указатель на юнита (или nullptr)
    Unit* GetUnit() const;

    // Ставит юнита в клетку
    void SetUnit(Unit* unit);

    // Убирает юнита из клетки и возвращает указатель на него (для перемещения)
    Unit* ReleaseUnit();

 private:
    Unit* unit_;
};

#endif  // CELL_H