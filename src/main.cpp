/**
 * @file main.cpp
 * @author Бессонов Андрей Олегович
 * @brief Точка входа в программу-сервер
 */
#include <iostream>
#include "Server.h"

/**
 * @brief Главная функция. Cоздает сервер и ловит исключения.
 */
int main(int argc, char** argv) {
    try {
        Server server;
        server.start(argc, argv); // Передаем аргументы командной строки
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
