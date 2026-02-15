#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory> // Для std::shared_ptr
#include "spell_interface.h"

class SpellHand {
 public:
    // Конструктор задает максимальный размер руки
    explicit SpellHand(int max_size);

    // Добавить заклинание (возвращает false, если рука полная)
    bool AddSpell(std::shared_ptr<ISpell> spell);

    // Получить заклинание по индексу (без удаления из руки)
    std::shared_ptr<ISpell> GetSpell(int index);

    // Удалить заклинание после использования
    void RemoveSpell(int index);

    // Текущее количество заклинаний
    int GetCount() const;

 private:
    int max_size_;
    std::vector<std::shared_ptr<ISpell>> spells_;
};

#endif  // SPELL_HAND_H