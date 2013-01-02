CC := g++
CFLAGS := -Wall -Wextra -g -pg -std=c++0x `sdl-config --cflags`
LIBS := -pg `sdl-config --libs` -lGL -lGLU -lSDL_image

OBJS := main.o Util.o TmxMap.o Map.o Texture.o \
	ResourceMgr.o Sprite.o Engine.o Game.o \
	GameObject.o Hero.o FpsCounter.o base64.o

VPATH=base64

all: marian

marian: $(OBJS) $(MAIN)
	$(CC) $^ -o $@ $(LIBS)

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CC) $< -o $@ -c $(CFLAGS)
	$(CC) $< -MM $(CFLAGS) > $*.d

.PHONY : test clean cleantmp
test:
	cd test; make test;

clean:
	rm -f marian $(OBJS) *.d
	cd test; make clean;

cleantmp:
	rm *~
