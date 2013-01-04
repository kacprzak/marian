CC           := g++
CFLAGS       := -Wall -Wextra -std=c++0x `sdl-config --cflags` -I.
LIBS         := -lGL -lGLU -lSDL_image `sdl-config --libs`
DEBUGFLAGS   := -O0 -g
RELEASEFLAGS := -O3 -DNDEBUG

TARGET       := marian

OBJS := main.o Util.o TmxMap.o Map.o Texture.o \
	ResourceMgr.o Sprite.o Engine.o Game.o \
	GameObject.o Hero.o FpsCounter.o base64.o

VPATH=src base64

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

clean:
	rm -f $(TARGET) $(OBJS) *.d

cleantmp:
	rm *~

.PHONY : clean cleantmp release profile
