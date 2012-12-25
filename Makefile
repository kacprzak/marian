marian: main.cpp
	g++ -o marian main.cpp Util.cpp `sdl-config --cflags --libs` -lGL -lGLU -lSDL_image -Wall -Wextra -g

all: marian

clean:
	rm marian