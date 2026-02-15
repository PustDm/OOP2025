#ifndef SPELLS_H
#define SPELLS_H

#include "spell_interface.h"

// Заклинание прямого урона (огненный шар)
class FireballSpell : public ISpell {
 public:
    bool Cast(int x, int y, Field& field) override;
    std::string GetName() const override;
};

// Заклинание урона по площади (землетрясение/буря)
class EarthquakeSpell : public ISpell {
 public:
    bool Cast(int x, int y, Field& field) override;
    std::string GetName() const override;
};

#endif  // SPELLS_H