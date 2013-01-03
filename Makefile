CC           := g++
CFLAGS       := -Wall -Wextra -std=c++0x `sdl-config --cflags`
LIBS         := -lGL -lGLU -lSDL_image `sdl-config --libs`
DEBUGFLAGS   := -O0 -g
RELEASEFLAGS := -O2 -DNDEBUG

TARGET       := marian

OBJS := main.o Util.o TmxMap.o Map.o Texture.o \
	ResourceMgr.o Sprite.o Engine.o Game.o \
	GameObject.o Hero.o FpsCounter.o base64.o

VPATH=base64

all: $(TARGET)

$(TARGET): CFLAGS += $(DEBUGFLAGS)
$(TARGET): $(OBJS) $(MAIN)
	$(CC) $^ -o $@ $(LIBS)

release: CFLAGS += $(RELEASEFLAGS)
release: $(OBJS) $(MAIN)
	$(CC) $^ -o $(TARGET) $(LIBS)

profile: CFLAGS += $(RELEASEFLAGS) -pg
profile: $(OBJS) $(MAIN)
	$(CC) $^ -o $(TARGET) $(LIBS) -pg

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CC) $< -o $@ -c $(CFLAGS)
	$(CC) $< -MM $(CFLAGS) > $*.d

test:
	cd test; make test;

clean:
	rm -f $(TARGET) $(OBJS) *.d
	cd test; make clean;

cleantmp:
	rm *~

.PHONY : test clean cleantmp release
