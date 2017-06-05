

CXXFLAGS=-std=c++11 -pthread -fdump-class-hierarchy -O2

TARGET=main

all:
		$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp task.cpp

clean:
		rm $(TARGET)
