/**
 * @file ClientDB.h
 * @brief База клиентов (логин:пароль)
 */
#pragma once
#include <string>
#include <unordered_map>

/**
 * @class ClientDB
 * @brief Хранит пары логин-пароль и позволяет их искать.
 */
class ClientDB {
public:
    /**
     * @brief Загружает базу из текстового файла.
     * @param path Путь к файлу.
     * @return true, если файл успешно прочитан.
     */
    bool loadFromFile(const std::string& path);

    /**
     * @brief Проверяет наличие пользователя.
     * @param login Логин.
     * @return true, если пользователь есть.
     */
    bool userExists(const std::string& login) const;

    /**
     * @brief Возвращает пароль пользователя.
     * @param login Логин.
     * @return Пароль или пустая строка.
     */
    std::string getPassword(const std::string& login) const;

private:
    std::unordered_map<std::string, std::string> users_; ///< Словарь логин -> пароль
};
