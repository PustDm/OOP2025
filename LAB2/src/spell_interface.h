#ifndef SPELL_INTERFACE_H
#define SPELL_INTERFACE_H

#include <string>

// Forward declaration, чтобы заклинание знало про поле
class Field; 

class ISpell {
 public:
    virtual ~ISpell() = default;
    
    // Основной метод использования заклинания
    // Возвращает true, если применение успешно (была цель и т.д.)
    virtual bool Cast(int x, int y, Field& field) = 0;
    
    // Имя для UI
    virtual std::string GetName() const = 0;
};

#endif  // SPELL_INTERFACE_H