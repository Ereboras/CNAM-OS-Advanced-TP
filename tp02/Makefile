CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
INC_DIR=./include
BIN_DIR=./bin

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)

all: exo1 exo2 exo3

exo1: $(SRC_DIR)/exo1.c
	gcc -o $(BIN_DIR)/exo1 $(SRC_DIR)/exo1.c

exo2: $(SRC_DIR)/exo2.c
	gcc -o $(BIN_DIR)/exo2 $(SRC_DIR)/exo2.c

exo3: $(SRC_DIR)/exo3.c
	gcc -o $(BIN_DIR)/exo3 $(SRC_DIR)/exo3.c

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(BIN_DIR)/*

.PHONY: doc
