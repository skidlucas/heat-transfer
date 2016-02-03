CC = gcc

OBJ_FICHIER = heatTransfer.c

EXE = heatTransfer

OPTION = -std=c11

all: $(EXE)
	
#Exécutable
heatTransfer:
	$(CC) $(OPTION) $(OBJ_FICHIER) -lm -o $(EXE)

clean: 
	rm -vf heatTransfer
