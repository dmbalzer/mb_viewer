SRC += src/*.c
FLG += -std=c99
FLG += -Wall
LNK += -lraylib
LNK += -lSDL2
LNK += -lmodbus
BIN += raygui_mb_viewer

build:
	gcc $(SRC) $(FLG) $(LNK) -o $(BIN)

run:
	./$(BIN)