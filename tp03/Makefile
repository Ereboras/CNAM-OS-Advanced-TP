CC=gcc
THREADFLAG=-pthread
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
INC_DIR=./include
BIN_DIR=./bin
DOC_DIR=./doc
GCOV_DIR=./gcov

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)
EXEC=skeleton

GEXEC=$(EXEC).cov

AR_NAME=archive_$(EXEC).tar.gz


all: a_exo1 b_exo1

a_exo1: $(SRC_DIR)/a_exo1.c
	$(CC) -o $(BIN_DIR)/a_exo1 $(SRC_DIR)/a_exo1.c

b_exo1: $(SRC_DIR)/b_exo1.c
	$(CC) $(THREADFLAG) -o $(BIN_DIR)/b_exo1 $(SRC_DIR)/b_exo1.c

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(BIN_DIR)/*
	rm -rf $(DOC_DIR)/latex/
	rm -rf $(DOC_DIR)/html/
	rm -rf $(GCOV_DIR)/*

.PHONY: doc
