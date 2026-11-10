#include "Authenticator.h"
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>

std::string Authenticator::generateSalt() {
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::byte salt[8]; // 8 байт = 64 бита
    prng.GenerateBlock(salt, sizeof(salt));

    std::string salt_hex;
    CryptoPP::StringSource(salt, sizeof(salt), true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(salt_hex)
        )
    );
    return salt_hex; // 16 символов
}

std::string Authenticator::computeMD5(const std::string& salt, const std::string& password) {
    std::string msg = salt + password;
    std::string hash_hex;

    CryptoPP::Weak::MD5 md5;
    CryptoPP::StringSource(msg, true,
        new CryptoPP::HashFilter(md5,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(hash_hex)
            )
        )
    );
    return hash_hex;
}

bool Authenticator::verify(const std::string& salt, const std::string& password, const std::string& expectedHash) {
    return computeMD5(salt, password) == expectedHash;
}
