#include "FamilyMember.h"       // наш заголовочный файл
#include "FamilyException.h"    // наш класс для ошибок
#include <cctype>               // нужен для isdigit (проверяем, что символ - цифра)

std::string FamilyMember::currentDate_ = "";    // static переменную надо один раз задать в cpp, дату потом вводим с клавиатуры

// ---------- Конструкторы и деструктор ----------

FamilyMember::FamilyMember()
    : fullName_(""), father_(""), mother_(""), spouse_(""), birthDate_(""),
    age_(0), children_(nullptr), childrenCount_(0)    // сразу задаём начальные значения полей
{
    std::cout << "[Default constructor] FamilyMember" << std::endl;     // выводим, что вызвался конструктор
}

FamilyMember::FamilyMember(const std::string& fullName, const std::string& father,
    const std::string& mother, const std::string& spouse,
    const std::string& birthDate)
    : age_(0), children_(nullptr), childrenCount_(0)
{
    setFullName(fullName);      // записываем через set-функции, в них есть проверки
    setFather(father);
    setMother(mother);
    setSpouse(spouse);
    setBirthDate(birthDate);
    std::cout << "[Parameter constructor] FamilyMember: " << fullName_ << std::endl;
}

FamilyMember::FamilyMember(const FamilyMember& other)
    : fullName_(other.fullName_), father_(other.father_), mother_(other.mother_),
    spouse_(other.spouse_), birthDate_(other.birthDate_), age_(other.age_),
    children_(nullptr), childrenCount_(other.childrenCount_)      // обычные поля просто копируем
{
    if (childrenCount_ > 0) {                                       // если у того, кого копируем, есть дети
        children_ = new std::string[childrenCount_];                // делаем свой новый массив
        for (int i = 0; i < childrenCount_; i++) {
            children_[i] = other.children_[i];                      // копируем имена по одному
        }
    }
    std::cout << "[Copy constructor] FamilyMember: " << fullName_ << std::endl;
}

FamilyMember::~FamilyMember() {
    std::cout << "[Destructor] FamilyMember: " << fullName_ << std::endl;
    delete[] children_;         // удаляем массив детей (для массивов пишем delete[])
}

// ---------- Операторы ----------

FamilyMember& FamilyMember::operator=(const FamilyMember& other) {
    if (this != &other) {                   // проверка, что не присваиваем объект самому себе (a = a)
        fullName_ = other.fullName_;        // обычные поля копируем
        father_ = other.father_;
        mother_ = other.mother_;
        spouse_ = other.spouse_;
        birthDate_ = other.birthDate_;
        age_ = other.age_;
        delete[] children_;                 // старый массив детей удаляем
        children_ = nullptr;
        childrenCount_ = other.childrenCount_;
        if (childrenCount_ > 0) {
            children_ = new std::string[childrenCount_];    // делаем новый массив такого же размера
            for (int i = 0; i < childrenCount_; i++) {
                children_[i] = other.children_[i];
            }
        }
    }
    return *this;                           // возвращаем сам объект
}

bool FamilyMember::operator==(const FamilyMember& other) const {
    return fullName_ == other.fullName_ && birthDate_ == other.birthDate_;   // считаем, что это один человек, если совпали ФИО и дата рождения
}

std::ostream& operator<<(std::ostream& out, const FamilyMember& member) {
    member.print(out);          // вызовется print того класса, к которому относится объект
    return out;
}

// ---------- Вспомогательные функции ----------

void FamilyMember::checkDate(const std::string& date) {
    bool formatOk = (date.length() == 10 && date[2] == '.' && date[5] == '.');   // длина 10 и точки стоят на своих местах
    for (int i = 0; formatOk && i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit((unsigned char)date[i])) {              // все остальные символы должны быть цифрами
            formatOk = false;
        }
    }
    if (!formatOk) {
        throw FamilyException("Wrong date format. Use DD.MM.YYYY, for example 05.03.1980");     // бросаем ошибку с текстом
    }
    int day = std::stoi(date.substr(0, 2));     // substr вырезает кусок строки (с какого символа, сколько), stoi делает из него число
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));
    int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };    // сколько дней в каждом месяце
    bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);     // проверка на високосный год
    if (leapYear) {
        daysInMonth[1] = 29;                    // в високосный год в феврале 29 дней
    }
    if (year < 1000 || month < 1 || month > 12 || day < 1 || day > daysInMonth[month - 1]) {
        throw FamilyException("This date does not exist: " + date);                      // такой даты нет, например 31.02.2000
    }
}

int FamilyMember::yearsBetween(const std::string& from, const std::string& to) {
    checkDate(from);            // сначала проверяем обе даты
    checkDate(to);
    int yearFrom = std::stoi(from.substr(6, 4));
    int yearTo = std::stoi(to.substr(6, 4));
    int monthDayFrom = std::stoi(from.substr(3, 2)) * 100 + std::stoi(from.substr(0, 2));   // месяц и день в одно число, например 5 марта это 305
    int monthDayTo = std::stoi(to.substr(3, 2)) * 100 + std::stoi(to.substr(0, 2));
    if (yearTo < yearFrom || (yearTo == yearFrom && monthDayTo < monthDayFrom)) {
        throw FamilyException("The second date is earlier than the first one: " + to + " < " + from);
    }
    int years = yearTo - yearFrom;              // разница в годах
    if (monthDayTo < monthDayFrom) {            // если день рождения в этом году ещё не наступил
        years--;                                // то отнимаем один год
    }
    return years;
}

void FamilyMember::setAge(int value) {
    age_ = value;
}

void FamilyMember::printCommon(std::ostream& out) const {
    out << "Full name: " << fullName_ << std::endl;
    out << "Father: " << (father_.empty() ? "unknown" : father_) << std::endl;     // так записывается короткий if/else
    out << "Mother: " << (mother_.empty() ? "unknown" : mother_) << std::endl;
    out << "Spouse: " << (spouse_.empty() ? "none" : spouse_) << std::endl;
    out << "Birth date: " << birthDate_ << std::endl;
    out << "Children: ";
    if (childrenCount_ == 0) {
        out << "none";
    }
    for (int i = 0; i < childrenCount_; i++) {
        out << children_[i];
        if (i < childrenCount_ - 1) {           // запятая между именами, после последнего не нужна
            out << ", ";
        }
    }
    out << std::endl;
}

// ---------- set / get ----------

void FamilyMember::setFullName(const std::string& value) {
    if (value.empty()) {                        // ФИО не должно быть пустым
        throw FamilyException("Full name must not be empty");
    }
    fullName_ = value;
}
void FamilyMember::setFather(const std::string& value) { father_ = value; }
void FamilyMember::setMother(const std::string& value) { mother_ = value; }
void FamilyMember::setSpouse(const std::string& value) { spouse_ = value; }
void FamilyMember::setBirthDate(const std::string& value) {
    checkDate(value);           // если дата неправильная, тут будет ошибка и дата не запишется
    birthDate_ = value;
}

std::string FamilyMember::getFullName() const { return fullName_; }
std::string FamilyMember::getFather() const { return father_; }
std::string FamilyMember::getMother() const { return mother_; }
std::string FamilyMember::getSpouse() const { return spouse_; }
std::string FamilyMember::getBirthDate() const { return birthDate_; }
int FamilyMember::getAge() const { return age_; }

// ---------- Дети ----------

void FamilyMember::addChild(const std::string& childName) {
    if (childName.empty()) {
        throw FamilyException("Child's name must not be empty");
    }
    std::string* newArray = new std::string[childrenCount_ + 1];    // делаем новый массив, на один элемент больше
    for (int i = 0; i < childrenCount_; i++) {
        newArray[i] = children_[i];                                 // переносим туда старых детей
    }
    newArray[childrenCount_] = childName;                           // нового ребёнка ставим в конец
    delete[] children_;                                             // старый массив уже не нужен, удаляем
    children_ = newArray;                                           // теперь указатель смотрит на новый массив
    childrenCount_++;                                               // детей стало на одного больше
}

void FamilyMember::removeChild(int index) {
    if (index < 0 || index >= childrenCount_) {                     // проверяем, что такой номер есть
        throw FamilyException("There is no child with this number");
    }
    if (childrenCount_ == 1) {                  // если ребёнок был один, то массив больше не нужен
        delete[] children_;
        children_ = nullptr;
        childrenCount_ = 0;
        return;                                 // выходим из функции
    }
    std::string* newArray = new std::string[childrenCount_ - 1];    // новый массив, на один элемент меньше
    int j = 0;                                                      // j - номер места в новом массиве
    for (int i = 0; i < childrenCount_; i++) {
        if (i != index) {                                           // того, кого удаляем, пропускаем
            newArray[j] = children_[i];
            j++;
        }
    }
    delete[] children_;
    children_ = newArray;
    childrenCount_--;
}

int FamilyMember::getChildrenCount() const { return childrenCount_; }

std::string FamilyMember::getChild(int index) const {
    if (index < 0 || index >= childrenCount_) {
        throw FamilyException("There is no child with this number");
    }
    return children_[index];
}

void FamilyMember::setChild(int index, const std::string& childName) {
    if (index < 0 || index >= childrenCount_) {
        throw FamilyException("There is no child with this number");
    }
    if (childName.empty()) {
        throw FamilyException("Child's name must not be empty");
    }
    children_[index] = childName;
}

// ---------- static-функции ----------

void FamilyMember::setCurrentDate(const std::string& value) {
    checkDate(value);
    currentDate_ = value;
}
std::string FamilyMember::getCurrentDate() { return currentDate_; }

std::string FamilyMember::readLine(std::istream& in) {
    std::string line;
    std::getline(in, line);                     // читаем строку целиком, вместе с пробелами
    if (!in) {                                  // не получилось прочитать - значит файл закончился раньше времени
        throw FamilyException("File reading error: unexpected end of data");
    }
    return line;
}

// ---------- Запись в файл и чтение из файла (общая часть) ----------

void FamilyMember::save(std::ostream& out) const {
    out << fullName_ << "\n";                   // каждое значение пишем с новой строки
    out << father_ << "\n";
    out << mother_ << "\n";
    out << spouse_ << "\n";
    out << birthDate_ << "\n";
    out << childrenCount_ << "\n";              // сначала пишем сколько детей, потом их имена
    for (int i = 0; i < childrenCount_; i++) {
        out << children_[i] << "\n";
    }
}

void FamilyMember::load(std::istream& in) {
    setFullName(readLine(in));                  // читаем в том же порядке, в каком записывали в save
    setFather(readLine(in));
    setMother(readLine(in));
    setSpouse(readLine(in));
    setBirthDate(readLine(in));
    int n = 0;                                  // сколько детей
    try {
        n = std::stoi(readLine(in));            // читаем строку и делаем из неё число
    }
    catch (const std::invalid_argument&) {    // stoi выдаёт эту ошибку, если в строке не число
        throw FamilyException("File error: number of children is not a number");
    }
    if (n < 0) {
        throw FamilyException("File error: negative number of children");
    }
    delete[] children_;                         // старый список детей очищаем
    children_ = nullptr;
    childrenCount_ = 0;
    for (int i = 0; i < n; i++) {
        addChild(readLine(in));                 // добавляем детей по одному
    }
}