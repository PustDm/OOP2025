#ifndef PLAYER_H
#define PLAYER_H

#include "unit.h"
#include "spell_hand.h"

class Player : public Unit {
 public:
    // Конструктор инициализирует базового юнита и размер руки
    Player(int health, int damage, int max_spells);

    // Управление очками
    void AddScore(int points);
    int GetScore() const;

    // Доступ к руке заклинаний (чтобы Игра могла вызывать Cast)
    SpellHand& GetSpellHand();

 private:
    int score_;
    SpellHand spell_hand_;
};

#endif  // PLAYER_H