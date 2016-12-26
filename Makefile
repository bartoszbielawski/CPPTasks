

CXXFLAGS=-std=c++11 -pthread

TARGET=main

all:
		$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
		rm $(TARGET)
