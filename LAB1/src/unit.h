#ifndef UNIT_H
#define UNIT_H

// Абстрактный базовый класс для всех существ
class Unit {
 public:
    // Конструктор инициализирует здоровье и урон
    Unit(int health, int damage);
    
    // Виртуальный деструктор обязателен для полиморфизма
    virtual ~Unit() = default;

    // Метод получения урона. Виртуальный, если захотим изменить поведение в будущем
    virtual void TakeDamage(int amount);

    // Проверка, жив ли юнит
    bool IsAlive() const;

    // Геттеры
    int GetDamage() const;
    int GetHealth() const;

 protected:
    int health_;
    int damage_;
};

#endif  // UNIT_H