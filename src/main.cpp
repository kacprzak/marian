/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include <iostream>

#include "ScriptMgr.h"
#include "Engine.h"
#include "Game.h"

int main(int argc, char *argv[])
{
    ScriptMgr& sm = ScriptMgr::instance();
    sm.setDataFolder("scripts/");
    sm.executeScript("startup.lua");

    int screenWidth = sm.getGlobalInt("screen_width");
    int screenHeight = sm.getGlobalInt("screen_height");
    bool fullScreen = sm.getGlobalBool("screen_full");

    Engine *e = new Engine("Marian",
                           screenWidth, screenHeight,
                           fullScreen);
    Game *game = new Game;

    e->mainLoop(game);

    delete e;
    delete game;

    return 0;
}
