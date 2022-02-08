CXX := g++
CFLAGS := -Wall
EXE_NAME := mh

all:
	$(CXX) $(CFLAGS) -o $(EXE_NAME) main.cpp

clean:
	$(RM) $(EXE_NAME)
