CC = g++
CFLAGS = -Wall -Wextra -g -std=c++0x `sdl-config --cflags`
LIBS = `sdl-config --libs` -lGL -lGLU -lSDL_image

OBJS = Util.o TmxMap.o Map.o Texture.o ResourceMgr.o Sprite.o Engine.o Game.o
MAIN = main.o

all: marian

marian: $(OBJS) $(MAIN)
	$(CC) $^ -o $@ $(LIBS)

.PHONY : test
test:
	cd test; make test;

$(OBJS): %.o: %.cpp %.h
	$(CC) $< -o $@ -c $(CFLAGS)

$(MAIN): %.o: %.cpp
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm marian $(OBJS)
	cd test; make clean;

cleantmp:
	rm *~
