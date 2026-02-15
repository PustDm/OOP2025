#include "spells.h"
#include "field.h"
#include "enemy.h"

// Реализация Fireball
bool FireballSpell::Cast(int x, int y, Field& field) {
    Unit* target = field.GetUnitAt(x, y);
    // Проверяем: есть ли цель и является ли она врагом
    // (Игрок не должен бить сам себя или пустые клетки этим заклинанием)
    if (target && dynamic_cast<Enemy*>(target)) {
        field.ApplyDamageAt(x, y, 50); // Фиксированный урон
        return true;
    }
    return false;
}

std::string FireballSpell::GetName() const {
    return "Fireball";
}

// Реализация Earthquake
bool EarthquakeSpell::Cast(int x, int y, Field& field) {
    // Бьет по площади всегда, даже если там пусто
    field.ApplyAreaDamage(x, y, 30);
    return true;
}

std::string EarthquakeSpell::GetName() const {
    return "Earthquake";
}