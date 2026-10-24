#ifndef FAMILYEXCEPTION_H       // чтобы файл не подключился два раза
#define FAMILYEXCEPTION_H

#include <stdexcept>            // тут есть runtime_error, от него наследуем свой класс ошибки
#include <string>               // для текста (std::string)

// Свой класс для ошибок. Если в программе что-то не так (неправильная дата, нет такого номера,
// файл не открылся), то "бросаем" (throw) его вместе с текстом, что случилось.
class FamilyException : public std::runtime_error {     // наследуем от runtime_error, он умеет хранить текст ошибки
private:
    // своих данных нет

protected:
    // тут тоже ничего нет

public:
    FamilyException(const std::string& message);        // конструктор, message - текст ошибки
    FamilyException(const FamilyException& other);      // конструктор копирования
    virtual ~FamilyException() noexcept;                // деструктор
};

#endif