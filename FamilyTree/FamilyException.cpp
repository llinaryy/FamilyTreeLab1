#include "FamilyException.h"    // подключаем наш заголовочный файл
#include <iostream>             // чтобы выводить на экран (cout)

// Конструктор: текст ошибки отдаём в runtime_error, он его запоминает. Потом через what() можно его получить
FamilyException::FamilyException(const std::string& message) : std::runtime_error(message) {
    std::cout << "[Parameter constructor] FamilyException" << std::endl;    // выводим, что вызвался конструктор
}

// Конструктор копирования: делает копию ошибки
FamilyException::FamilyException(const FamilyException& other) : std::runtime_error(other) {
    std::cout << "[Copy constructor] FamilyException" << std::endl;
}

// Деструктор: срабатывает, когда ошибку уже обработали
FamilyException::~FamilyException() noexcept {
    std::cout << "[Destructor] FamilyException" << std::endl;
}