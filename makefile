CC=gcc -std=c99
CFLAGS= −W −Wall
EXEC=GPMaps
#SRC:= $(wildcard *.c)
SRC:= $(wildcard src/*.c)
#SRC:= $(wildcard **/*.c)
OBJ= $(SRC:.c=.o)
LIBS=

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -o $@ -c $^

.PHONY: clean

clean:
	rm -rf $(OBJ)

destroy: clean
	rm -f $(TARGETS)