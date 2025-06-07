CXX = g++
CXXFLAGS = -Wall -Wpedantic -Wextra -std=c++20 -Iheaders
TARGET = main
SRC_DIR = src
SRC = main.cpp $(wildcard $(SRC_DIR)/*.cpp)

.PHONY: all clean run rebuild valgrind

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

clean:
	rm -f $(TARGET) *.bin $(SRC_DIR)/*.bin 

rebuild: clean all