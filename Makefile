# Makefile for Parking System
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = ParkingSystem
SRCS = main.cpp Car.cpp Stack.cpp Queue.cpp ParkingSystem.cpp ConsoleGUI.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: all clean run test
