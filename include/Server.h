/**
 * @file Server.h
 * @brief Основной класс TCP-сервера
 */
#pragma once
#include <string>
#include <atomic>
#include "ClientDB.h"
#include "Logger.h"

class Server {
public:
    Server();
    ~Server();

    // Запуск сервера с разбором аргументов
    void start(int argc, char** argv); 
    
    void stop();

private:
    void handleClient(int clientSock);
    bool authenticateClient(int clientSock);
    void parseArgs(int argc, char** argv); // Новый метод

    int port_;
    int listenSock_ = -1;
    std::atomic<bool> running_{false};
    std::string dbPath_;
    ClientDB db_;
    Logger* logger_; // Указатель, так как логгер создается после парсинга
};
