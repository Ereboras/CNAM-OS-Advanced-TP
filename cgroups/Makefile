SRC_DIR=./src
BIN_DIR=./bin

exo1: $(SRC_DIR)/exo1.o
	gcc -o $(BIN_DIR)/exo1 $(SRC_DIR)/exo1.o

exo1.o: $(SRC_DIR)/exo1.c
	gcc -o $(SRC_DIR)/exo1.o -c $(SRC_DIR)/exo1.c -Wall

clean:
	rm -rf $(SRC_DIR)/*.o