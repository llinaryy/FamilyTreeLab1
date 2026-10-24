#include <iostream>                 // для ввода и вывода (cout, cin)
#include <string>                   // для строк (std::string), stoi и to_string
#include "Keeper.h"                 // наши классы (в кавычках - наши файлы из проекта)
#include "LivingMember.h"
#include "DeceasedMember.h"
#include "FamilyException.h"

// Выводим подсказку и читаем с клавиатуры всю строку целиком (с пробелами, например ФИО)
std::string inputString(const std::string& prompt) {
    std::cout << prompt;                // выводим подсказку
    std::string line;                   // сюда запишем то, что введём
    std::getline(std::cin, line);       // getline читает всю строку до нажатия Enter
    return line;
}

// Читаем целое число. Если ввели не число, выдаём ошибку с понятным текстом
int inputInt(const std::string& prompt) {
    std::string line = inputString(prompt);
    try {
        return std::stoi(line);                 // stoi делает из строки число
    }
    catch (...) {                             // три точки значит "поймать любую ошибку"
        throw FamilyException("Please enter a whole number");
    }
}

// Вводим номер члена семьи (мы считаем с 1, а программа с 0, поэтому отнимаем 1)
int inputIndex(const Keeper& keeper) {
    int number = inputInt("Enter the number of the family member (1-" + std::to_string(keeper.getCount()) + "): ");
    return number - 1;
}

// Добавляем живого члена семьи, все данные вводим с клавиатуры
void addLiving(Keeper& keeper) {
    std::string name = inputString("Full name: ");
    std::string father = inputString("Father's full name (Enter if unknown): ");
    std::string mother = inputString("Mother's full name (Enter if unknown): ");
    std::string spouse = inputString("Spouse's full name (Enter if none): ");
    std::string birth = inputString("Birth date (DD.MM.YYYY): ");
    keeper.add(new LivingMember(name, father, mother, spouse, birth));    // new создаёт объект в динамической памяти, add кладёт его в контейнер
    std::cout << "Living family member added." << std::endl;
}

// Добавляем умершего члена семьи
void addDeceased(Keeper& keeper) {
    std::string name = inputString("Full name: ");
    std::string father = inputString("Father's full name (Enter if unknown): ");
    std::string mother = inputString("Mother's full name (Enter if unknown): ");
    std::string spouse = inputString("Spouse's full name (Enter if none): ");
    std::string birth = inputString("Birth date (DD.MM.YYYY): ");
    std::string death = inputString("Death date (DD.MM.YYYY): ");
    keeper.add(new DeceasedMember(name, father, mother, spouse, birth, death));
    std::cout << "Deceased family member added." << std::endl;
}

// Меню, где меняем данные одного члена семьи
void editMember(FamilyMember* member) {
    std::cout << "1 - Full name\n2 - Father\n3 - Mother\n4 - Spouse\n5 - Birth date\n"
        << "6 - Add a child\n7 - Remove a child\n8 - Death date (deceased only)\n9 - Rename a child\n";
    int choice = inputInt("What to change: ");
    if (choice == 1) {
        member->setFullName(inputString("New full name: "));            // стрелка -> вызывает функцию объекта по указателю
    }
    else if (choice == 2) {
        member->setFather(inputString("Father's full name: "));
    }
    else if (choice == 3) {
        member->setMother(inputString("Mother's full name: "));
    }
    else if (choice == 4) {
        member->setSpouse(inputString("Spouse's full name: "));
    }
    else if (choice == 5) {
        std::string oldDate = member->getBirthDate();                   // запоминаем старую дату
        member->setBirthDate(inputString("New birth date (DD.MM.YYYY): "));
        try {
            member->calculateAge();                                     // дата поменялась, значит возраст считаем заново
        }
        catch (...) {
            member->setBirthDate(oldDate);                              // если новая дата не подходит (например позже смерти), возвращаем старую
            throw;                                                      // и показываем ошибку
        }
    }
    else if (choice == 6) {
        member->addChild(inputString("Child's full name: "));
    }
    else if (choice == 7) {
        if (member->getChildrenCount() == 0) {
            throw FamilyException("This person has no children");
        }
        for (int i = 0; i < member->getChildrenCount(); i++) {
            std::cout << i + 1 << " - " << member->getChild(i) << std::endl;    // показываем детей с номерами
        }
        member->removeChild(inputInt("Number of the child to remove: ") - 1);
    }
    else if (choice == 8) {
        // dynamic_cast проверяет, что объект правда DeceasedMember.
        // Если нет (человек живой), то получится nullptr (пустой указатель).
        DeceasedMember* dead = dynamic_cast<DeceasedMember*>(member);
        if (dead == nullptr) {
            throw FamilyException("A living person has no death date");
        }
        std::string oldDate = dead->getDeathDate();
        dead->setDeathDate(inputString("New death date (DD.MM.YYYY): "));
        try {
            dead->calculateAge();
        }
        catch (...) {
            dead->setDeathDate(oldDate);                                // если дата не подходит, возвращаем старую
            throw;
        }
    }
    else if (choice == 9) {
        if (member->getChildrenCount() == 0) {
            throw FamilyException("This person has no children");
        }
        for (int i = 0; i < member->getChildrenCount(); i++) {
            std::cout << i + 1 << " - " << member->getChild(i) << std::endl;
        }
        int number = inputInt("Number of the child to rename: ");
        member->setChild(number - 1, inputString("New full name of the child: "));
    }
    else {
        throw FamilyException("There is no such menu item");
    }
    std::cout << "Data changed." << std::endl;
}

int main() {
    Keeper keeper;                      // наш контейнер (обычная переменная, в конце программы удалится сам)

    // Сначала вводим сегодняшнюю дату, она нужна, чтобы считать возраст живых
    bool dateOk = false;
    while (!dateOk) {                   // повторяем, пока не введём правильную дату
        try {
            FamilyMember::setCurrentDate(inputString("Enter today's date (DD.MM.YYYY): "));
            dateOk = true;              // если ошибки не было, значит дата правильная
        }
        catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << std::endl;    // what() выдаёт текст ошибки
        }
    }

    bool running = true;                // пока true, меню показывается снова и снова
    while (running) {
        std::cout << "\n===== FAMILY TREE =====\n"
            << "1 - Add a living family member\n"
            << "2 - Add a deceased family member\n"
            << "3 - Remove a family member\n"
            << "4 - Show all family members\n"
            << "5 - Edit a family member\n"
            << "6 - Save to a file\n"
            << "7 - Load from a file\n"
            << "8 - Copy test (copy constructor of the whole list)\n"
            << "9 - Compare two family members (operator ==)\n"
            << "0 - Exit\n";
        try {                           // всё, что может выдать ошибку, находится внутри try
            int choice = inputInt("Your choice: ");
            if (choice == 1) {
                addLiving(keeper);
            }
            else if (choice == 2) {
                addDeceased(keeper);
            }
            else if (choice == 3) {
                keeper.printAll();      // сначала показываем всех, чтобы выбрать номер
                if (keeper.getCount() > 0) {
                    keeper.remove(inputIndex(keeper));
                    std::cout << "Removed." << std::endl;
                }
            }
            else if (choice == 4) {
                keeper.printAll();
            }
            else if (choice == 5) {
                keeper.printAll();
                if (keeper.getCount() > 0) {
                    editMember(keeper[inputIndex(keeper)]);     // keeper[...] - это наш оператор []
                }
            }
            else if (choice == 6) {
                keeper.saveToFile(inputString("File name to save (for example family.txt): "));
                std::cout << "Saved." << std::endl;
            }
            else if (choice == 7) {
                keeper.loadFromFile(inputString("File name to load: "));
                std::cout << "Loaded." << std::endl;
            }
            else if (choice == 8) {
                Keeper copy(keeper);                            // тут вызывается конструктор копирования Keeper
                std::cout << "The copy is created. Its content:" << std::endl;
                copy.printAll();
            }
            else if (choice == 9) {
                if (keeper.getCount() < 2) {
                    throw FamilyException("Two family members are needed for comparison");
                }
                keeper.printAll();
                FamilyMember* first = keeper[inputIndex(keeper)];
                FamilyMember* second = keeper[inputIndex(keeper)];
                if (*first == *second) {                        // звёздочка даёт сам объект, значит сравниваем объекты, а не указатели
                    std::cout << "It is the same person (same full name and birth date)." << std::endl;
                }
                else {
                    std::cout << "These are different people." << std::endl;
                }
            }
            else if (choice == 0) {
                running = false;        // цикл while закончится
            }
            else {
                std::cout << "There is no such menu item." << std::endl;
            }
        }
        catch (const std::exception& error) {                 // ловим любую ошибку и выводим её текст
            std::cout << "Error: " << error.what() << std::endl;
        }
    }
    std::cout << "Goodbye." << std::endl;
    return 0;                           // при выходе сработает деструктор keeper и удалит всех членов семьи
}