/**
 * @file Config.h
 * @author Бессонов Андрей Олегович
 * @version 1.0
 * @date 2026
 * @brief Заголовочный файл с конфигурационными константами сервера
 *
 * @details Содержит константы по умолчанию для порта, файлов,
 * размеров данных и сообщений протокола.
 * @warning Изменение констант может повлиять на совместимость с клиентами.
 */

#pragma once
#include <cstdint>

namespace Config {
    constexpr int DEFAULT_PORT = 33333;
    constexpr const char* DEFAULT_DB_PATH = "/etc/vcalc.conf";
    constexpr const char* DEFAULT_LOG_PATH = "/var/log/vcalc.log";

    // Служебные сообщения протокола (ТЗ п. 4.2.3)
    constexpr const char* MSG_OK  = "OK\n";
    constexpr const char* MSG_ERR = "ERR\n";
}
