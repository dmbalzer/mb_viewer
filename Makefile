SRC += src/*.c
FLG += -std=c99
FLG += -Wall
LNK += -lopengl32
LNK += -lgdi32
BIN += mb_viewer

build:
	gcc $(SRC) $(FLG) $(LNK) -o $(BIN)

run:
	./$(BIN)

