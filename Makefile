SRC += src/*.c
FLG += -std=c99
FLG += -Wall
LIB += -lraylib
BIN += mb_viewer

build:
	gcc $(SRC) $(FLG) $(LIB) -o $(BIN)

run:
	./$(BIN)

