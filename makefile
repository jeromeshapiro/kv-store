CC = g++
FLAGS = -std=c++14
SRC = src/main.cpp
DEPS = src/hashMap.h src/hashMapNode.h
TARGET = bin/app

all: $(SRC) $(DEPS)
	$(CC) $(FLAGS) -o $(TARGET) $(SRC)

