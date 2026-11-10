/**
 * @file main.cpp
 * @author Бессонов Андрей Олегович
 * @brief Точка входа в программу-сервер
 */
#include <iostream>
#include "Server.h"
#include "UserInterface.h"

/**
 * @brief Главная функция. Создает сервер и обрабатывает исключения.
 */
int main(int argc, char** argv) {
    try {
        // 1. Парсим аргументы
        Params params = UserInterface::parse(argc, argv);

        // 2. Создаем сервер с готовыми параметрами
        Server server(params);

        // 3. Запускаем
        server.start();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
