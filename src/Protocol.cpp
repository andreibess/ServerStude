#include "Protocol.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

static bool readAll(int sock, void* buf, size_t n) {
    char* p = static_cast<char*>(buf);
    size_t left = n;
    while (left > 0) {
        ssize_t r = recv(sock, p, left, 0);
        if (r <= 0) return false;
        p += r;
        left -= r;
    }
    return true;
}

static bool writeAll(int sock, const void* buf, size_t n) {
    const char* p = static_cast<const char*>(buf);
    size_t left = n;
    while (left > 0) {
        ssize_t r = send(sock, p, left, 0);
        if (r <= 0) return false;
        p += r;
        left -= r;
    }
    return true;
}

bool Protocol::sendUint32(int sock, uint32_t value) {
    uint32_t net_val = htonl(value);
    return writeAll(sock, &net_val, sizeof(net_val));
}

bool Protocol::recvUint32(int sock, uint32_t& value) {
    uint32_t net_val;
    if (!readAll(sock, &net_val, sizeof(net_val))) return false;
    value = ntohl(net_val);
    return true;
}

bool Protocol::sendDouble(int sock, double value) {
    uint64_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    bits = htobe64(bits);
    return writeAll(sock, &bits, sizeof(bits));
}

bool Protocol::recvDouble(int sock, double& value) {
    uint64_t bits;
    if (!readAll(sock, &bits, sizeof(bits))) return false;
    bits = be64toh(bits);
    std::memcpy(&value, &bits, sizeof(value));
    return true;
}

bool Protocol::sendString(int sock, const std::string& str) {
    return writeAll(sock, str.data(), str.size());
}

bool Protocol::recvLine(int sock, std::string& line) {
    line.clear();
    char c;
    while (true) {
        ssize_t r = recv(sock, &c, 1, 0);
        if (r <= 0) return false;
        if (c == '\n') return true;
        line.push_back(c);
    }
}
