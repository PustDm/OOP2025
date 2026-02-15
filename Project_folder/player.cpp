#include "player.h"

Player::Player(int health, int damage, int max_spells)
    : Unit(health, damage),       // Инициализация базового класса
      score_(0),                  // Очки по умолчанию 0
      spell_hand_(max_spells) {}  // Инициализация объекта SpellHand

void Player::AddScore(int points) {
    score_ += points;
}

int Player::GetScore() const {
    return score_;
}

SpellHand& Player::GetSpellHand() {
    return spell_hand_;
}