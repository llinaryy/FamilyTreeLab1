#include "DeceasedMember.h"

DeceasedMember::DeceasedMember() : FamilyMember(), deathDate_("") {
    std::cout << "[Default constructor] DeceasedMember" << std::endl;
}

DeceasedMember::DeceasedMember(const std::string& fullName, const std::string& father,
    const std::string& mother, const std::string& spouse,
    const std::string& birthDate, const std::string& deathDate)
    : FamilyMember(fullName, father, mother, spouse, birthDate)     // общие данные отдаём в базовый класс
{
    setDeathDate(deathDate);    // проверяем и записываем дату смерти
    calculateAge();             // считаем возраст на момент смерти
    std::cout << "[Parameter constructor] DeceasedMember: " << getFullName() << std::endl;
}

DeceasedMember::DeceasedMember(const DeceasedMember& other)
    : FamilyMember(other), deathDate_(other.deathDate_)             // базовую часть копирует базовый класс, а дату смерти копируем сами
{
    std::cout << "[Copy constructor] DeceasedMember: " << getFullName() << std::endl;
}

DeceasedMember::~DeceasedMember() {
    std::cout << "[Destructor] DeceasedMember: " << getFullName() << std::endl;
}

void DeceasedMember::setDeathDate(const std::string& value) {
    checkDate(value);           // неправильная дата не запишется
    deathDate_ = value;
}

std::string DeceasedMember::getDeathDate() const { return deathDate_; }

void DeceasedMember::calculateAge() {
    setAge(yearsBetween(getBirthDate(), deathDate_));       // будет ошибка, если смерть раньше рождения
}

void DeceasedMember::print(std::ostream& out) const {
    out << "--- Deceased family member ---" << std::endl;
    printCommon(out);
    out << "Death date: " << deathDate_ << std::endl;
    out << "Age at death: " << getAge() << std::endl;
}

std::string DeceasedMember::getType() const { return "DECEASED"; }

FamilyMember* DeceasedMember::clone() const {
    return new DeceasedMember(*this);
}

void DeceasedMember::save(std::ostream& out) const {
    FamilyMember::save(out);            // сначала общая часть
    out << deathDate_ << "\n";          // потом то, что есть только у умерших
}

void DeceasedMember::load(std::istream& in) {
    FamilyMember::load(in);             // сначала общая часть (порядок такой же, как в save)
    setDeathDate(readLine(in));
    calculateAge();
}