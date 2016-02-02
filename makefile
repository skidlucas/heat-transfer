CC = gcc

OBJ_FICHIER = heatTransfer.c

EXE = heatTransfer

all: $(EXE)
	
#Exécutable
heatTransfer:
	$(CC) $(OBJ_FICHIER) -o $(EXE)

clean: 
	rm -vf heatTransfer
