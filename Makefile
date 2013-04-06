CFLAGS=-Wall -W -Werror -O6
BIN=jsme
SRC=jsme.c

all: $(BIN)
$(BIN): $(SRC)
clean:
	rm -f $(BIN)
