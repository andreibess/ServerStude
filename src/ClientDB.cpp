#include "ClientDB.h"
#include <fstream>

bool ClientDB::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            users_[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }
    return true;
}

bool ClientDB::userExists(const std::string& login) const {
    return users_.find(login) != users_.end();
}

std::string ClientDB::getPassword(const std::string& login) const {
    auto it = users_.find(login);
    return (it != users_.end()) ? it->second : "";
}
