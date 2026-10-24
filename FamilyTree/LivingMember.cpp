#include "LivingMember.h"
#include "FamilyException.h"

LivingMember::LivingMember() : FamilyMember() {     // сначала вызывается конструктор базового класса
    std::cout << "[Default constructor] LivingMember" << std::endl;
}

LivingMember::LivingMember(const std::string& fullName, const std::string& father,
    const std::string& mother, const std::string& spouse,
    const std::string& birthDate)
    : FamilyMember(fullName, father, mother, spouse, birthDate)     // данные отдаём в базовый класс
{
    calculateAge();             // сразу считаем возраст
    std::cout << "[Parameter constructor] LivingMember: " << getFullName() << std::endl;
}

LivingMember::LivingMember(const LivingMember& other) : FamilyMember(other) {   // базовую часть копирует конструктор копирования базового класса
    std::cout << "[Copy constructor] LivingMember: " << getFullName() << std::endl;
}

LivingMember::~LivingMember() {
    std::cout << "[Destructor] LivingMember: " << getFullName() << std::endl;
    // деструктор базового класса вызовется сам после этого
}

void LivingMember::calculateAge() {
    if (getCurrentDate().empty()) {                 // если не ввели сегодняшнюю дату, возраст не посчитать
        throw FamilyException("Today's date is not set");
    }
    setAge(yearsBetween(getBirthDate(), getCurrentDate()));     // будет ошибка, если дата рождения позже сегодняшней
}

void LivingMember::print(std::ostream& out) const {
    out << "--- Living family member ---" << std::endl;
    printCommon(out);                               // общие данные выводит функция из базового класса
    out << "Age: " << getAge() << std::endl;
}

std::string LivingMember::getType() const { return "LIVING"; }

FamilyMember* LivingMember::clone() const {
    return new LivingMember(*this);     // создаём новый объект копированием, *this - это сам текущий объект
}

void LivingMember::load(std::istream& in) {
    FamilyMember::load(in);             // читаем общую часть (функция базового класса)
    calculateAge();                     // возраст в файле не хранится, поэтому считаем заново
}