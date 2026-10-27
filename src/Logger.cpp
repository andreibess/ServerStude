#include "Logger.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

Logger::Logger(const std::string& path) {
    file_.open(path, std::ios::app);
    if (!file_.is_open()) {
        std::cerr << "Warning: Cannot open log file: " << path << std::endl;
    }
}

void Logger::log(const std::string& message, Severity severity) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::time_t now = std::time(nullptr);
    std::tm tm_now;
    localtime_r(&now, &tm_now);

    std::ostringstream oss;
    oss << "[" << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "] "
        << (severity == Severity::CRITICAL ? "[CRIT] " : "[INFO] ")
        << message << "\n";

    if (file_.is_open()) {
        file_ << oss.str();
        file_.flush();
    }
}
