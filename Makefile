

CXXFLAGS=-std=c++11 -pthread -fdump-class-hierarchy -O2

TARGET=main

all:
		$(CXX) $(CXXFLAGS) -o $(TARGET) task.cpp main.cpp 

clean:
		rm $(TARGET)
