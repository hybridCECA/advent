CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = program

all: $(TARGET)

$(TARGET): main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
