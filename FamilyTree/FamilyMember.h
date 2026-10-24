#ifndef FAMILYMEMBER_H          // чтобы файл не подключился два раза
#define FAMILYMEMBER_H

#include <string>               // для текста (std::string)
#include <iostream>             // для потоков ввода и вывода (экран и файл)

// Абстрактный базовый класс "Член семьи" (в задании он называется Base).
// Он абстрактный, потому что в нём есть функции с "= 0" (чисто виртуальные).
// Из-за этого объект именно этого класса создать нельзя, можно только объекты наследников.
class FamilyMember {
private:                                        // private - эти данные видно только внутри самого класса
    std::string fullName_;                      // ФИО (подчёркивание в конце, как в методичке для полей)
    std::string father_;                        // ФИО отца (пусто - значит неизвестно)
    std::string mother_;                        // ФИО матери (пусто - значит неизвестно)
    std::string spouse_;                        // ФИО супруга или супруги (пусто - нет)
    std::string birthDate_;                     // дата рождения, формат ДД.ММ.ГГГГ
    int age_;                                   // возраст в годах, считаем сами по датам
    std::string* children_;                     // указатель на массив с именами детей (массив динамический)
    int childrenCount_;                         // сколько детей сейчас в массиве
    static std::string currentDate_;            // сегодняшняя дата, static - одна на все объекты

protected:                                      // protected - видно в классе и в наследниках
    static void checkDate(const std::string& date);                             // проверка даты, если неверная - ошибка
    static int yearsBetween(const std::string& from, const std::string& to);    // сколько полных лет между двумя датами
    void setAge(int value);                                                     // возраст записывают только наследники
    void printCommon(std::ostream& out) const;                                  // вывод общих данных, они у всех одинаковые

public:                                         // public - доступно отовсюду
    FamilyMember();                                         // конструктор без параметров
    FamilyMember(const std::string& fullName, const std::string& father,
        const std::string& mother, const std::string& spouse,
        const std::string& birthDate);             // конструктор с параметрами
    FamilyMember(const FamilyMember& other);                // конструктор копирования
    virtual ~FamilyMember();                                // деструктор, virtual нужен чтобы наследники тоже правильно удалялись

    FamilyMember& operator=(const FamilyMember& other);     // оператор присваивания (a = b)
    bool operator==(const FamilyMember& other) const;       // оператор сравнения (a == b)

    void setFullName(const std::string& value);             // set-функции, чтобы записать значение
    void setFather(const std::string& value);
    void setMother(const std::string& value);
    void setSpouse(const std::string& value);
    void setBirthDate(const std::string& value);
    std::string getFullName() const;                        // get-функции, чтобы посмотреть значение
    std::string getFather() const;
    std::string getMother() const;
    std::string getSpouse() const;
    std::string getBirthDate() const;
    int getAge() const;

    void addChild(const std::string& childName);            // добавить ребёнка
    void removeChild(int index);                            // удалить ребёнка по номеру
    int getChildrenCount() const;                           // сколько детей
    std::string getChild(int index) const;                  // имя ребёнка по номеру
    void setChild(int index, const std::string& childName); // поменять имя ребёнка

    static void setCurrentDate(const std::string& value);   // записываем сегодняшнюю дату
    static std::string getCurrentDate();
    static std::string readLine(std::istream& in);          // прочитать одну строку (из файла), с проверкой

    virtual void calculateAge() = 0;                        // посчитать возраст, у каждого наследника по-своему
    virtual void print(std::ostream& out) const = 0;        // вывод на экран, у каждого наследника свой
    virtual std::string getType() const = 0;                // название типа, пишем его в файл
    virtual FamilyMember* clone() const = 0;                // сделать копию объекта такого же типа

    virtual void save(std::ostream& out) const;             // записать данные в файл
    virtual void load(std::istream& in);                    // прочитать данные из файла
};

std::ostream& operator<<(std::ostream& out, const FamilyMember& member);   // чтобы можно было писать cout << member;

#endif