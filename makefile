CC = g++
CFLAGS = $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)
SRC = main.cpp player.cpp obstacle.cpp bullet.cpp
OUT = game

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
