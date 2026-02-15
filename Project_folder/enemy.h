#ifndef ENEMY_H
#define ENEMY_H

#include "unit.h"

// Класс Врага наследуется от Unit
class Enemy : public Unit {
 public:
    Enemy(int health, int damage);
};

#endif  // ENEMY_H