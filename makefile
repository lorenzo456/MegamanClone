# Compiler
CC = g++

# Source files
SRC = main.cpp player.cpp obstacle.cpp bullet.cpp enemy.cpp boss.cpp
OUT = game

# Detect OS
ifeq ($(OS),Windows_NT)
    # Windows settings
    OUT := $(OUT).exe
    RAYLIB_INC = C:/raylib/include
    RAYLIB_LIB = C:/raylib/lib
    CFLAGS = -I$(RAYLIB_INC) -std=c++17 -Wall
    LDFLAGS = -L$(RAYLIB_LIB) -lraylib -lopengl32 -lgdi32 -lwinmm -lshlwapi
    RM = del /f
else
    # Linux settings
    CFLAGS = $(shell pkg-config --cflags raylib) -std=c++17 -Wall
    LDFLAGS = $(shell pkg-config --libs raylib)
    RM = rm -f
endif

# Build
all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Clean
clean:
	$(RM) $(OUT)
