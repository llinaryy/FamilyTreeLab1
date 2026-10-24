#include "Keeper.h"
#include "LivingMember.h"       // нужны, чтобы при чтении из файла создавать объекты этих классов
#include "DeceasedMember.h"
#include "FamilyException.h"
#include <fstream>              // для работы с файлами: ofstream - запись, ifstream - чтение

Keeper::Keeper() : items_(nullptr), count_(0) {     // пустой контейнер: массива нет, элементов 0
    std::cout << "[Default constructor] Keeper" << std::endl;
}

Keeper::Keeper(FamilyMember* first) : items_(nullptr), count_(0) {
    std::cout << "[Parameter constructor] Keeper" << std::endl;
    add(first);                 // добавляем через уже написанную функцию add
}

Keeper::Keeper(const Keeper& other) : items_(nullptr), count_(other.count_) {
    if (count_ > 0) {
        items_ = new FamilyMember * [count_];             // новый массив указателей
        for (int i = 0; i < count_; i++) {
            items_[i] = other.items_[i]->clone();       // clone делает настоящую копию объекта (живого или умершего)
        }
    }
    std::cout << "[Copy constructor] Keeper" << std::endl;
}

Keeper::~Keeper() {
    std::cout << "[Destructor] Keeper" << std::endl;
    clear();                    // удаляем всех членов семьи и сам массив
}

Keeper& Keeper::operator=(const Keeper& other) {
    if (this != &other) {                               // проверка, что не присваиваем самому себе
        clear();                                        // сначала очищаем то, что было
        count_ = other.count_;
        if (count_ > 0) {
            items_ = new FamilyMember * [count_];
            for (int i = 0; i < count_; i++) {
                items_[i] = other.items_[i]->clone();   // копируем каждого члена семьи
            }
        }
    }
    return *this;
}

FamilyMember* Keeper::operator[](int index) const {
    return get(index);          // get проверит номер, и если он неправильный, выдаст ошибку
}

void Keeper::add(FamilyMember* member) {
    if (member == nullptr) {
        throw FamilyException("Cannot add an empty object");
    }
    FamilyMember** newItems = new FamilyMember * [count_ + 1];    // новый массив, на один элемент больше
    for (int i = 0; i < count_; i++) {
        newItems[i] = items_[i];                                // переносим старые указатели
    }
    newItems[count_] = member;                                  // нового члена семьи ставим в конец
    delete[] items_;                                            // удаляем только старый массив указателей, сами объекты остаются
    items_ = newItems;
    count_++;
}

void Keeper::remove(int index) {
    if (index < 0 || index >= count_) {
        throw FamilyException("There is no family member with this number");
    }
    delete items_[index];                                       // удаляем сам объект (сработает его деструктор)
    if (count_ == 1) {                                          // если он был один, массив больше не нужен
        delete[] items_;
        items_ = nullptr;
        count_ = 0;
        return;
    }
    FamilyMember** newItems = new FamilyMember * [count_ - 1];    // новый массив, на один элемент меньше
    int j = 0;
    for (int i = 0; i < count_; i++) {
        if (i != index) {                                       // удалённого пропускаем
            newItems[j] = items_[i];
            j++;
        }
    }
    delete[] items_;
    items_ = newItems;
    count_--;
}

void Keeper::clear() {
    for (int i = 0; i < count_; i++) {
        delete items_[i];                                       // удаляем каждый объект
    }
    delete[] items_;                                            // удаляем массив указателей
    items_ = nullptr;
    count_ = 0;
}

int Keeper::getCount() const { return count_; }

FamilyMember* Keeper::get(int index) const {
    if (index < 0 || index >= count_) {
        throw FamilyException("There is no family member with this number");
    }
    return items_[index];
}

void Keeper::printAll() const {
    if (count_ == 0) {
        std::cout << "The list is empty." << std::endl;
        return;
    }
    for (int i = 0; i < count_; i++) {
        std::cout << "Number " << i + 1 << std::endl;           // номера для человека начинаем с 1
        std::cout << *items_[i] << std::endl;                   // тут работает наш operator<<
    }
}

void Keeper::saveToFile(const std::string& fileName) const {
    std::ofstream out(fileName.c_str());                        // открываем файл для записи
    if (!out.is_open()) {                                       // если не открылся
        throw FamilyException("Cannot open the file for writing: " + fileName);
    }
    out << count_ << "\n";                                      // в первой строке пишем, сколько записей
    for (int i = 0; i < count_; i++) {
        out << items_[i]->getType() << "\n";                    // тип записи: LIVING или DECEASED
        items_[i]->save(out);                                   // остальное объект записывает сам
    }
}

void Keeper::loadFromFile(const std::string& fileName) {
    std::ifstream in(fileName.c_str());                         // открываем файл для чтения
    if (!in.is_open()) {                                        // если файла нет или он не открылся
        throw FamilyException("Cannot open the file for reading: " + fileName);
    }
    Keeper temp;                                                // сначала читаем во временный контейнер,
    // чтобы если файл плохой, не потерять то, что уже есть
    int n = 0;                                                  // сколько записей в файле
    try {
        n = std::stoi(FamilyMember::readLine(in));
    }
    catch (const std::invalid_argument&) {
        throw FamilyException("File error: the first line must be a number");
    }
    for (int i = 0; i < n; i++) {
        std::string type = FamilyMember::readLine(in);          // читаем тип записи
        FamilyMember* member = nullptr;
        if (type == "LIVING") {
            member = new LivingMember();                        // создаём пустой объект нужного типа
        }
        else if (type == "DECEASED") {
            member = new DeceasedMember();
        }
        else {
            throw FamilyException("File error: unknown type " + type);
        }
        try {
            member->load(in);                                   // объект сам читает свои данные из файла
        }
        catch (...) {                                         // если что-то пошло не так
            delete member;                                      // удаляем объект, чтобы не было утечки памяти
            throw;                                              // и отправляем ту же ошибку дальше
        }
        temp.add(member);
    }
    clear();                                                    // всё прочитали хорошо, теперь очищаем старые данные
    items_ = temp.items_;                                       // забираем массив у временного контейнера
    count_ = temp.count_;
    temp.items_ = nullptr;                                      // у временного контейнера обнуляем, чтобы его деструктор
    temp.count_ = 0;                                            // не удалил наших членов семьи
}