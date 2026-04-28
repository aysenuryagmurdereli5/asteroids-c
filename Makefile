CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lmingw32 -lSDL2main -lSDL2
SRC = src/main.c
OBJ = asteroids.exe

all:
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OBJ)

clean:
	rm -f $(OBJ)

	