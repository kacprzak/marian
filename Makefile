CC = g++
CFLAGS = -Wall -Wextra -g `sdl-config --cflags`
LIBS = `sdl-config --libs` -lGL -lGLU -lSDL_image

OBJS = main.o Util.o Map.o

all: marian

marian: $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

map_test: Map.cpp map_test.cpp
	$(CC) $^ -o $@ -lboost_unit_test_framework	

test: map_test
	./map_test

$(OBJS): %.o: %.cpp
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm marian $(OBJS) map_test

cleantmp:
	rm *~