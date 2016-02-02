CC = gcc

OBJ_FICHIER = heatTransfer.c

EXE = heatTransfer

OPTION = 

all: $(EXE)
	
#Exécutable
heatTransfer:
	$(CC) $(OPTION) $(OBJ_FICHIER) -o $(EXE)

clean: 
	rm -vf heatTransfer
