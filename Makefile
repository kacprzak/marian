CC = g++
CFLAGS = -Wall -Wextra -g -std=c++0x `sdl-config --cflags`
LIBS = `sdl-config --libs` -lGL -lGLU -lSDL_image

OBJS = Util.o Map.o Texture.o ResourceMgr.o Sprite.o Engine.o Game.o
MAIN = main.o

all: marian

marian: $(OBJS) $(MAIN)
	$(CC) $^ -o $@ $(LIBS)

map_test: Map.cpp Map.h map_test.cpp
	$(CC) $^ -o $@ -lboost_unit_test_framework	

test: map_test
	./map_test

$(OBJS): %.o: %.cpp %.h
	$(CC) $< -o $@ -c $(CFLAGS)

$(MAIN): %.o: %.cpp
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm marian $(OBJS) map_test

cleantmp:
	rm *~
