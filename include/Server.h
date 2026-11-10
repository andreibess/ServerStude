/**
 * @file Server.h
 * @brief Основной класс TCP-сервера
 */
#pragma once
#include <string>
#include <atomic>
#include "ClientDB.h"
#include "Logger.h"
#include "UserInterface.h"

/**
 * @class Server
 * @brief Управляет жизненным циклом сервера и обработкой клиентов.
 */
class Server {
public:
    /**
     * @brief Конструктор сервера.
     * @param params Структура с параметрами запуска (порт, пути).
     */
    Server(const Params& params);

    /** @brief Деструктор, останавливает сервер. */
    ~Server();

    /**
     * @brief Запускает сервер (блокирующий вызов).
     * @return true, если сервер успешно запустился.
     */
    bool start();

    /** @brief Останавливает сервер. */
    void stop();

private:
    /** @brief Обрабатывает подключение одного клиента. */
    void handleClient(int clientSock);

    /** @brief Проводит аутентификацию клиента. */
    bool authenticateClient(int clientSock);

    int port_;                          ///< Порт
    int listenSock_ = -1;               ///< Сокет прослушивания
    std::atomic<bool> running_{false};  ///< Флаг работы
    std::string dbPath_;                ///< Путь к БД
    ClientDB db_;                       ///< База клиентов
    Logger* logger_;                    ///< Журнал (указатель, т.к. создается в конструкторе)
};
