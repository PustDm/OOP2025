#include "field.h"
#include "player.h" // Будет создан в след. шаге, но нужен для копирования
#include "enemy.h"  // Нужен для копирования и проверки коллизий
#include "common.h"
#include <algorithm> // для std::swap

// Конструктор
Field::Field(int width, int height) : width_(width), height_(height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw GameException("Invalid field size: must be between 10x10 and 25x25");
    }
    AllocateCells(width_, height_);
}

// Деструктор
Field::~Field() {
    FreeCells();
}

// Выделение памяти под массив клеток
void AllocateCells(int w, int h) {
    // Этот метод внутри класса, поэтому cells_ доступен
}

void Field::AllocateCells(int w, int h) {
    cells_ = new Cell*[h];
    for (int i = 0; i < h; ++i) {
        cells_[i] = new Cell[w];
    }
}

// Очистка памяти
void Field::FreeCells() {
    if (cells_) {
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                // Важно: Поле владеет юнитами, удаляем их
                Unit* u = cells_[i][j].ReleaseUnit();
                if (u) delete u;
            }
            delete[] cells_[i];
        }
        delete[] cells_;
        cells_ = nullptr;
    }
}

// Конструктор копирования (Deep Copy)
Field::Field(const Field& other) : width_(other.width_), height_(other.height_) {
    AllocateCells(width_, height_);
    DeepCopyFrom(other);
}

// Логика глубокого копирования
void Field::DeepCopyFrom(const Field& other) {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Unit* src = other.cells_[y][x].GetUnit();
            if (src) {
                // Определяем тип юнита и создаем его копию
                if (auto p = dynamic_cast<Player*>(src)) {
                    cells_[y][x].SetUnit(new Player(*p));
                } else if (auto e = dynamic_cast<Enemy*>(src)) {
                    cells_[y][x].SetUnit(new Enemy(*e));
                }
            }
        }
    }
}

// Конструктор перемещения
Field::Field(Field&& other) noexcept : width_(0), height_(0), cells_(nullptr) {
    std::swap(width_, other.width_);
    std::swap(height_, other.height_);
    std::swap(cells_, other.cells_);
}

// Оператор присваивания копированием
Field& Field::operator=(const Field& other) {
    if (this != &other) {
        FreeCells();
        width_ = other.width_;
        height_ = other.height_;
        AllocateCells(width_, height_);
        DeepCopyFrom(other);
    }
    return *this;
}

// Оператор присваивания перемещением
Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        FreeCells();
        width_ = 0;
        height_ = 0;
        cells_ = nullptr;
        
        std::swap(width_, other.width_);
        std::swap(height_, other.height_);
        std::swap(cells_, other.cells_);
    }
    return *this;
}

bool Field::PlaceUnit(Unit* unit, int x, int y) {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) return false;
    if (cells_[y][x].IsOccupied()) return false;
    cells_[y][x].SetUnit(unit);
    return true;
}

bool Field::MoveUnit(int from_x, int from_y, int to_x, int to_y) {
    // Проверка границ
    if (to_x < 0 || to_y < 0 || to_x >= width_ || to_y >= height_) return false;
    
    Unit* mover = cells_[from_y][from_x].GetUnit();
    if (!mover) return false;

    Unit* target = cells_[to_y][to_x].GetUnit();
    
    // Если целевая клетка занята
    if (target) {
        // Логика по заданию: Враг идет на игрока -> урон, но не перемещение
        if (dynamic_cast<Enemy*>(mover) && dynamic_cast<Player*>(target)) {
            target->TakeDamage(mover->GetDamage());
            return true; // Действие (атака) совершено
        }
        // В остальных случаях (Враг в Врага, Игрок в Врага) просто нельзя пройти
        return false; 
    }

    // Если свободно - перемещаем
    cells_[to_y][to_x].SetUnit(cells_[from_y][from_x].ReleaseUnit());
    return true;
}

Unit* Field::GetUnitAt(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) return nullptr;
    return cells_[y][x].GetUnit();
}

void Field::RemoveDeadUnit(int x, int y) {
    Unit* u = cells_[y][x].ReleaseUnit();
    if (u) delete u;
}

void Field::ApplyDamageAt(int x, int y, int damage) {
    Unit* u = GetUnitAt(x, y);
    if (u) {
        u->TakeDamage(damage);
        if (!u->IsAlive()) {
            RemoveDeadUnit(x, y);
        }
    }
}

void Field::ApplyAreaDamage(int center_x, int center_y, int damage) {
    // Область 2x2 от указанной точки (вправо и вниз)
    for (int y = center_y; y < center_y + 2; ++y) {
        for (int x = center_x; x < center_x + 2; ++x) {
            ApplyDamageAt(x, y, damage);
        }
    }
}

int Field::GetWidth() const { return width_; }
int Field::GetHeight() const { return height_; }

std::vector<Position> Field::GetEnemyPositions() const {
    std::vector<Position> positions;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (cells_[y][x].GetUnit() && dynamic_cast<Enemy*>(cells_[y][x].GetUnit())) {
                positions.push_back({x, y});
            }
        }
    }
    return positions;
}