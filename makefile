CC = g++
SRC = src/main.cpp
TARGET = bin/test

all: $(SRC)
	$(CC) -o $(TARGET) $(SRC)

