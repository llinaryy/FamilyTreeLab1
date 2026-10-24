#ifndef DECEASEDMEMBER_H
#define DECEASEDMEMBER_H

#include "FamilyMember.h"

// Класс-наследник: умерший член семьи (есть дата смерти).
class DeceasedMember : public FamilyMember {
private:
    std::string deathDate_;                                 // дата смерти ДД.ММ.ГГГГ (есть только у этого класса)

protected:
    // тут ничего нет

public:
    DeceasedMember();
    DeceasedMember(const std::string& fullName, const std::string& father,
        const std::string& mother, const std::string& spouse,
        const std::string& birthDate, const std::string& deathDate);
    DeceasedMember(const DeceasedMember& other);
    virtual ~DeceasedMember();

    void setDeathDate(const std::string& value);            // set и get для даты смерти
    std::string getDeathDate() const;

    virtual void calculateAge() override;                   // возраст = сколько полных лет от рождения до смерти
    virtual void print(std::ostream& out) const override;
    virtual std::string getType() const override;
    virtual FamilyMember* clone() const override;
    virtual void save(std::ostream& out) const override;    // дописываем в файл дату смерти
    virtual void load(std::istream& in) override;           // читаем из файла дату смерти
};

#endif