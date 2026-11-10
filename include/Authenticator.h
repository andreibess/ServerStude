/**
 * @file Authenticator.h
 * @brief Модуль аутентификации (MD5 + SALT)
 */
#pragma once
#include <string>

/**
 * @class Authenticator
 * @brief Статические методы для генерации SALT и проверки MD5.
 */
class Authenticator {
public:
    /**
     * @brief Генерирует SALT16 (64 бита в hex-формате).
     * @return Строка из 16 шестнадцатеричных цифр.
     */
    static std::string generateSalt();

    /**
     * @brief Вычисляет MD5(SALT + PASSWORD).
     * @param salt Соль.
     * @param password Пароль.
     * @return Хеш в виде hex-строки.
     */
    static std::string computeMD5(const std::string& salt, const std::string& password);

    /**
     * @brief Проверяет соответствие хеша.
     * @param salt Соль.
     * @param password Пароль.
     * @param expectedHash Ожидаемый хеш.
     * @return true, если хеши совпали.
     */
    static bool verify(const std::string& salt, const std::string& password, const std::string& expectedHash);
};
