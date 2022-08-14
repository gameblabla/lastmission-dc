
APP_NAME = last-mission-sdl

# compiler

CC = gcc

CFLAGS = -D__DINGUX__ -DSOUND -Wall -Os -std=c99 -fms-extensions
LFLAGS = -lSDL -lSDL_mixer -lm

# source files

OBJ =	m_data.o
OBJ +=	demo.o enemies.o engine.o input.o main.o random.o room.o \
	sound.o sprites.o video.o world.o

all : $(APP_NAME)

$(APP_NAME) : $(OBJ)
	$(CC) $^ $(LFLAGS) -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	rm -rf ./*.o $(APP_NAME)
