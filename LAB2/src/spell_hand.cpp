#include "spell_hand.h"

SpellHand::SpellHand(int max_size) : max_size_(max_size) {}

bool SpellHand::AddSpell(std::shared_ptr<ISpell> spell) {
    if (spells_.size() >= max_size_) {
        return false;
    }
    spells_.push_back(spell);
    return true;
}

std::shared_ptr<ISpell> SpellHand::GetSpell(int index) {
    if (index >= 0 && index < spells_.size()) {
        return spells_[index];
    }
    return nullptr;
}

void SpellHand::RemoveSpell(int index) {
    if (index >= 0 && index < spells_.size()) {
        spells_.erase(spells_.begin() + index);
    }
}

int SpellHand::GetCount() const {
    return spells_.size();
}