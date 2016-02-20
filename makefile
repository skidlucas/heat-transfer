SRC=src
BIN=bin

CC = gcc
CCFLAGS = -g -std=c11 -Wall
CC_COMPILE = $(CC) $(CCFLAGS)

EXEC = ./bin/heatTransfer

SRC_FILES = $(wildcard $(SRC)/*.c)
OBJ_FILES = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SRC_FILES))

# Targets

all: $(EXEC)


$(EXEC): $(OBJ_FILES)
	$(CC_COMPILE) $^ -o $@ -lm

$(OBJ_FILES): $(BIN)/%.o : $(SRC)/%.c
	- mkdir -vp $(BIN)
	$(CC_COMPILE) -c $< -o $@

# Clean

clean:
	rm -rf $(BIN)

