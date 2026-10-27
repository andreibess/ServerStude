#include "Protocol.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>
#include <errno.h>

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
    return writeAll(sock, &value, sizeof(value));
}

bool Protocol::recvUint32(int sock, uint32_t& value) {
    return readAll(sock, &value, sizeof(value));
}

bool Protocol::sendDouble(int sock, double value) {
    return writeAll(sock, &value, sizeof(value));
}

bool Protocol::recvDouble(int sock, double& value) {
    return readAll(sock, &value, sizeof(value));
}

bool Protocol::sendString(int sock, const std::string& str) {
    return writeAll(sock, str.data(), str.size());
}

bool Protocol::recvLine(int sock, std::string& line) {
    line.clear();
    char c;

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (true) {
        ssize_t r = recv(sock, &c, 1, 0);

        if (r < 0) {
            if ((errno == EAGAIN || errno == EWOULDBLOCK) && !line.empty()) break;
            return false;
        }
        if (r == 0) return false;
        if (c == '\n' || c == '\r') break;
        line.push_back(c);
    }

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    return true;
}
