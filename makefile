CC = gcc

OBJ_FICHIER = heatTransfer.c

EXE = heatTransfer

OPTION = -std=c11 -Wall

SRC = ./src/

BIN = ./bin/

all: $(EXE)
	
#Exécutable
heatTransfer: heatTransfer.o
	$(CC) $(OPTION) $(BIN)heatTransfer.o $(BIN)matrice.o $(SRC)main.c -lm -o $(BIN)$(EXE)

matrice.o:
	$(CC) $(OPTION) -c $(SRC)matrice.c -l $(SRC)matrice.h  -lm -o $(BIN)matrice.o

heatTransfer.o: matrice.o
	$(CC) $(OPTION) -c $(SRC)heatTransfer.c -l $(SRC)heatTransfer.h $(BIN)matrice.o -lm -o $(BIN)heatTransfer.o 

clean: 
	rm -vf ./bin/*
