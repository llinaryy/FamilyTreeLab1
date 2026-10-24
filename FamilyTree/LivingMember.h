#ifndef LIVINGMEMBER_H
#define LIVINGMEMBER_H

#include "FamilyMember.h"       // нужен базовый класс

// Класс-наследник: живой член семьи (даты смерти нет).
class LivingMember : public FamilyMember {      // public-наследование: живой член семьи - это тоже член семьи
private:
    // своих данных нет

protected:
    // тут ничего нет

public:
    LivingMember();                                         // без параметров
    LivingMember(const std::string& fullName, const std::string& father,
        const std::string& mother, const std::string& spouse,
        const std::string& birthDate);             // с параметрами
    LivingMember(const LivingMember& other);                // копирования
    virtual ~LivingMember();                                // деструктор

    virtual void calculateAge() override;                   // возраст = сколько полных лет от рождения до сегодня; override - переписываем функцию из базового класса
    virtual void print(std::ostream& out) const override;   // свой вывод на экран
    virtual std::string getType() const override;           // название типа для файла
    virtual FamilyMember* clone() const override;           // копия такого же типа
    virtual void load(std::istream& in) override;           // после чтения из файла считаем возраст заново
};

#endif