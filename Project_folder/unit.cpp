#include "unit.h"

Unit::Unit(int health, int damage) : health_(health), damage_(damage) {}

void Unit::TakeDamage(int amount) {
    health_ -= amount;
    if (health_ < 0) {
        health_ = 0;
    }
}

bool Unit::IsAlive() const {
    return health_ > 0;
}

int Unit::GetDamage() const {
    return damage_;
}

int Unit::GetHealth() const {
    return health_;
}