SRC += src/*.c
FLG += -std=c99
FLG += -Wall
LNK += -lraylibdll
LNK += -lmodbus
BIN += raygui_mb_viewer

build:
	gcc $(SRC) $(FLG) $(LNK) -o $(BIN)

run:
	./$(BIN)