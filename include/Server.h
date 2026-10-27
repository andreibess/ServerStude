/**
 * @file Server.h
 * @brief Основной класс TCP-сервера
 */
#pragma once
#include <string>
#include <atomic>
#include "ClientDB.h"
#include "Logger.h"

/**
 * @class Server
 * @brief Управляет жизненным циклом сервера и обработкой клиентов.
 */
class Server {
public:
    Server(int port, const std::string& dbPath, const std::string& logPath);
    ~Server();

    bool start();
    void stop();

private:
    void handleClient(int clientSock);
    bool authenticateClient(int clientSock);

    int port_;
    int listenSock_ = -1;
    std::atomic<bool> running_{false};
    std::string dbPath_;
    ClientDB db_;
    Logger logger_;
};
