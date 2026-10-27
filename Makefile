# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lcryptopp -lboost_program_options

# Директории
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/server

# Поиск всех .cpp файлов в src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Замена .cpp на .o для папки obj
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Цель по умолчанию
all: $(TARGET)

# Линковка исполняемого файла
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Сборка завершена: $(TARGET)"

# Компиляция каждого .cpp в .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка сгенерированных файлов
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
	@echo "Очистка завершена"

# Пересборка с нуля
rebuild: clean all

.PHONY: all clean rebuild
