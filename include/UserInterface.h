/**
 * @file UserInterface.h
 * @brief Разбор параметров командной строки
 */
#pragma once
#include <string>

/**
 * @struct Params
 * @brief Структура для хранения параметров запуска.
 */
struct Params {
    std::string dbPath;  ///< Путь к базе клиентов
    std::string logPath; ///< Путь к логу
    int port;            ///< Порт
};

/**
 * @class UserInterface
 * @brief Парсер командной строки на базе boost::program_options.
 */
class UserInterface {
public:
    /**
     * @brief Разбирает аргументы командной строки.
     * @param argc Количество аргументов.
     * @param argv Массив аргументов.
     * @return Структура Params с результатами.
     * @throw std::exception Если аргументы некорректны.
     */
    static Params parse(int argc, char** argv);
};
