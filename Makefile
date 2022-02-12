CXX := g++
CFLAGS := -Wall -std=c++17
EXE_NAME := mh

all:
	$(CXX) $(CFLAGS) -o $(EXE_NAME) main.cpp

clean:
	$(RM) $(EXE_NAME)
