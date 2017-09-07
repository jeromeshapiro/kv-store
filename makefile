CC = g++
FLAGS = -std=c++11
SRC = src/main.cpp src/hashMap.cpp
TARGET = bin/test

all: $(SRC)
	$(CC) $(FLAGS) -o $(TARGET) $(SRC)

