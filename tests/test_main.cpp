#include <UnitTest++/UnitTest++.h>
#include <fstream>
#include <cstdio>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>

// Заголовки тестируемых классов
#include "VectorProcessor.h"
#include "ClientDB.h"
#include "Authenticator.h"
#include "Logger.h"
#include "Protocol.h"
#include "UserInterface.h"
#include "Config.h"

// ==========================================
// Тестирование VectorProcessor
// ==========================================

TEST(VectorProcessor_Positive) {
    std::vector<double> vec = {1.0, 2.0, 3.0};
    double expected = 14.0;
    CHECK_EQUAL(expected, VectorProcessor::sumOfSquares(vec));
}

TEST(VectorProcessor_Negative) {
    std::vector<double> vec = {-2.0, -3.0};
    double expected = 13.0;
    CHECK_EQUAL(expected, VectorProcessor::sumOfSquares(vec));
}

TEST(VectorProcessor_Double) {
    std::vector<double> vec = {1.1, 2.2};
    double expected = 1.1 * 1.1 + 2.2 * 2.2;
    CHECK_EQUAL(expected, VectorProcessor::sumOfSquares(vec));
}

TEST(VectorProcessor_Empty) {
    std::vector<double> vec = {};
    CHECK_EQUAL(0.0, VectorProcessor::sumOfSquares(vec));
}

// ==========================================
// Тестирование ClientDB
// ==========================================

// Фикстура для создания временного файла БД
struct TestDBFixture {
    const std::string filename = "test_db.txt";
    TestDBFixture() {
        std::ofstream f(filename);
        f << "user:123\n";
        f << "admin:adminpass\n";
    }
    ~TestDBFixture() {
        std::remove(filename.c_str());
    }
};

TEST_FIXTURE(TestDBFixture, TestDB_LoadValid) {
    ClientDB db;
    CHECK(db.loadFromFile(filename));
    CHECK(db.userExists("user"));
    CHECK_EQUAL("123", db.getPassword("user"));
}

TEST(TestDB_LoadNoFile) {
    ClientDB db;
    CHECK(!db.loadFromFile("not_found.txt"));
}

TEST_FIXTURE(TestDBFixture, TestDB_UserExists) {
    ClientDB db;
    db.loadFromFile(filename);
    CHECK(!db.userExists("unknown"));
}

// ==========================================
// Тестирование Authenticator
// ==========================================

TEST(Authenticator_SaltLength) {
    std::string salt = Authenticator::generateSalt();
    CHECK_EQUAL(16, salt.length());
}

TEST(Authenticator_MD5Hash) {
    std::string salt = "1234567890123456";
    std::string pass = "password";
    std::string hash = Authenticator::computeMD5(salt, pass);
    CHECK_EQUAL(32, hash.length()); // MD5 всегда 32 символа в hex
}

TEST(Authenticator_VerifyCorrect) {
    std::string salt = "1234567890123456";
    std::string pass = "password";
    std::string hash = Authenticator::computeMD5(salt, pass);
    CHECK(Authenticator::verify(salt, pass, hash));
}

TEST(Authenticator_VerifyWrong) {
    std::string salt = "1234567890123456";
    std::string pass = "password";
    std::string wrongPass = "wrong";
    std::string hash = Authenticator::computeMD5(salt, pass);
    CHECK(!Authenticator::verify(salt, wrongPass, hash));
}

// ==========================================
// Тестирование Logger
// ==========================================

TEST(Logger_WriteAndRead) {
    std::string filename = "test_log.log";
    {
        Logger logger(filename);
        logger.log("Test message", Severity::NON_CRITICAL);
        logger.log("Critical error", Severity::CRITICAL);
    }
    
    std::ifstream file(filename);
    std::string line;
    
    std::getline(file, line);
    CHECK(line.find("[INFO] Test message") != std::string::npos);
    
    std::getline(file, line);
    CHECK(line.find("[CRIT] Critical error") != std::string::npos);
    
    std::remove(filename.c_str());
}

// ==========================================
// Тестирование Protocol
// ==========================================

TEST(Protocol_Uint32AndDouble) {
    int sv[2]; // Пара сокетов (имитация соединения без сети)
    CHECK_EQUAL(0, socketpair(AF_UNIX, SOCK_STREAM, 0, sv));

    // Тест uint32_t
    Protocol::sendUint32(sv[0], 4);
    uint32_t n = 0;
    Protocol::recvUint32(sv[1], n);
    CHECK_EQUAL(4, n);

    // Тест double
    Protocol::sendDouble(sv[0], 152.46);
    double d = 0.0;
    Protocol::recvDouble(sv[1], d);
    CHECK_EQUAL(152.46, d);

    close(sv[0]);
    close(sv[1]);
}

TEST(Protocol_String) {
    int sv[2];
    CHECK_EQUAL(0, socketpair(AF_UNIX, SOCK_STREAM, 0, sv));

    Protocol::sendString(sv[0], "user");
    Protocol::sendString(sv[0], "\n"); 
    
    std::string line;
    Protocol::recvLine(sv[1], line);
    CHECK_EQUAL("user", line);

    close(sv[0]);
    close(sv[1]);
}

// ==========================================
// Тестирование UserInterface
// ==========================================

TEST(UI_ParseCustomArgs) {
    const char* argv[] = {"test", "-d", "my_db.txt", "-l", "my_log.log", "-p", "5000", nullptr};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    Params p = UserInterface::parse(argc, const_cast<char**>(argv));

    CHECK_EQUAL("my_db.txt", p.dbPath);
    CHECK_EQUAL("my_log.log", p.logPath);
    CHECK_EQUAL(5000, p.port);
}

TEST(UI_ParseDefaults) {
    const char* argv[] = {"test"};
    int argc = 1;
    
    Params p = UserInterface::parse(argc, const_cast<char**>(argv));
    
    CHECK_EQUAL(Config::DEFAULT_DB_PATH, p.dbPath);
    CHECK_EQUAL(Config::DEFAULT_PORT, p.port);
}

// Запуск всех тестов
int main() {
    return UnitTest::RunAllTests();
}
