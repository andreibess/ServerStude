/**
 * @file Logger.h
 * @brief Класс для ведения журнала работы программы
 */
#pragma once
#include <string>
#include <fstream>
#include <mutex>

/** @brief Уровень критичности ошибки */
enum class Severity {
    CRITICAL,     ///< Критическая ошибка
    NON_CRITICAL  ///< Некритическая ошибка/событие
};

/**
 * @class Logger
 * @brief Обеспечивает потокобезопасную запись в лог-файл.
 */
class Logger {
public:
    /**
     * @brief Конструктор, открывает файл для записи.
     * @param path Путь к файлу журнала.
     */
    explicit Logger(const std::string& path);
    
    /**
     * @brief Записывает сообщение в журнал.
     * @param message Текст сообщения.
     * @param severity Уровень критичности.
     */
    void log(const std::string& message, Severity severity);

private:
    std::ofstream file_; ///< Файловый поток
    std::mutex mtx_;     ///< Мьютекс
};
