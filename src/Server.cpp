#include "Server.h"
#include "Protocol.h"
#include "VectorProcessor.h"
#include "Config.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>

// Статические функции для аутентификации
static std::string generateSalt() {
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::byte salt[8];
    prng.GenerateBlock(salt, sizeof(salt));
    std::string hex;
    CryptoPP::StringSource(salt, sizeof(salt), true,
        new CryptoPP::HexEncoder(new CryptoPP::StringSink(hex)));
    return hex;
}

static std::string computeMD5(const std::string& salt, const std::string& pass) {
    std::string msg = salt + pass;
    std::string hash;
    CryptoPP::MD5 md5;
    CryptoPP::StringSource(msg, true,
        new CryptoPP::HashFilter(md5, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
    return hash;
}

Server::Server(int port, const std::string& dbPath, const std::string& logPath)
    : port_(port), dbPath_(dbPath), logger_(logPath) {}

Server::~Server() { stop(); }

bool Server::start() {
    if (!db_.loadFromFile(dbPath_)) {
        logger_.log("Failed to load DB: " + dbPath_, Severity::CRITICAL);
        return false;
    }

    listenSock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock_ < 0) return false;

    int opt = 1;
    setsockopt(listenSock_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(listenSock_, (sockaddr*)&addr, sizeof(addr)) < 0) return false;
    if (listen(listenSock_, 5) < 0) return false;

    running_ = true;
    logger_.log("Server started on port " + std::to_string(port_), Severity::NON_CRITICAL);

    while (running_) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSock = accept(listenSock_, (sockaddr*)&clientAddr, &clientLen);
        if (clientSock >= 0) {
            handleClient(clientSock);
            close(clientSock);
        }
    }
    logger_.log("Сервер остановлен", Severity::NON_CRITICAL);
    return true;
}

void Server::stop() {
    running_ = false;
    if (listenSock_ >= 0) {
        close(listenSock_);
        listenSock_ = -1;
    }
}

bool Server::authenticateClient(int clientSock) {
    std::string login;
    if (!Protocol::recvLine(clientSock, login)) return false;

    if (!db_.userExists(login)) {
        Protocol::sendString(clientSock, Config::MSG_ERR);
        logger_.log("Unknown login: " + login, Severity::NON_CRITICAL);
        return false;
    }

    std::string salt = generateSalt();
    Protocol::sendString(clientSock, salt + "\n");

    std::string clientHash;
    if (!Protocol::recvLine(clientSock, clientHash)) return false;

    if (computeMD5(salt, db_.getPassword(login)) != clientHash) {
        Protocol::sendString(clientSock, Config::MSG_ERR);
        logger_.log("Auth failed for: " + login, Severity::NON_CRITICAL);
        return false;
    }

    Protocol::sendString(clientSock, Config::MSG_OK);
    return true;
}

void Server::handleClient(int clientSock) {
    if (!authenticateClient(clientSock)) return;

    uint32_t n;
    if (!Protocol::recvUint32(clientSock, n)) return;

    for (uint32_t i = 0; i < n; ++i) {
        uint32_t size;
        if (!Protocol::recvUint32(clientSock, size)) return;

        std::vector<double> vec(size);
        for (uint32_t j = 0; j < size; ++j) {
            if (!Protocol::recvDouble(clientSock, vec[j])) return;
        }

        double result = VectorProcessor::sumOfSquares(vec);
        Protocol::sendDouble(clientSock, result);
    }
    logger_.log("Session finished", Severity::NON_CRITICAL);
}
