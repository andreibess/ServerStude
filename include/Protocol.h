/**
 * @file Protocol.h
 * @brief Функции сетевого протокола
 */
#pragma once
#include <cstdint>
#include <string>

/**
 * @class Protocol
 * @brief Набор статических функций для сериализации данных.
 */
class Protocol {
public:
    static bool sendUint32(int sock, uint32_t value);
    static bool recvUint32(int sock, uint32_t& value);
    static bool sendDouble(int sock, double value);
    static bool recvDouble(int sock, double& value);
    static bool sendString(int sock, const std::string& str);
    static bool recvLine(int sock, std::string& line);
};
