/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include <iostream>

#include "Engine.h"
#include "Game.h"

int main()
{
    Engine *e = new Engine("Marian", 1024, 768);
    Game *game = new Game;

    e->mainLoop(game);

    delete e;
    delete game;

    return 0;
}
