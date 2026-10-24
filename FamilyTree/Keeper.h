#ifndef KEEPER_H
#define KEEPER_H

#include <string>
#include "FamilyMember.h"

// Контейнер, в котором лежат члены семьи. Без STL, свой динамический массив указателей.
class Keeper {
private:
    FamilyMember** items_;      // массив указателей на членов семьи (две звёздочки - это указатель на указатели)
    int count_;                 // сколько членов семьи сейчас хранится

protected:
    // тут ничего нет

public:
    Keeper();                                       // пустой контейнер
    Keeper(FamilyMember* first);                    // контейнер, в который сразу кладём одного члена семьи
    Keeper(const Keeper& other);                    // конструктор копирования (копирует всех членов семьи)
    ~Keeper();                                      // деструктор, удаляет всех членов семьи

    Keeper& operator=(const Keeper& other);         // оператор присваивания
    FamilyMember* operator[](int index) const;      // keeper[0] вернёт первого члена семьи

    void add(FamilyMember* member);                 // добавить
    void remove(int index);                         // удалить по номеру (номера с нуля)
    void clear();                                   // удалить всех
    int getCount() const;                           // сколько сейчас хранится
    FamilyMember* get(int index) const;             // взять по номеру
    void printAll() const;                          // показать всех

    void saveToFile(const std::string& fileName) const;     // сохранить всё в файл
    void loadFromFile(const std::string& fileName);         // загрузить всё из файла
};

#endif