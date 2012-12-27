#include <iostream>

#include "Engine.h"
#include "Game.h"

int main()
{
  Engine e("Marian", 640, 640);
  Game game;

  e.mainLoop(&game);

  return 0;
}
