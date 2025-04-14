CC = gcc
CFLAGS = -Wall -g
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXEC = tema1

build: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: build clean
