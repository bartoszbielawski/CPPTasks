

CXXFLAGS=-std=c++11 -pthread

all:
		$(CXX) $(CXXFLAGS) -o main main.cpp
