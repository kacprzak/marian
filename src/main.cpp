/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include <iostream>

#include "ScriptMgr.h"
#include "Engine.h"
#include "Game.h"

int main(int argc, char *argv[])
{
    new ScriptMgr;
    ScriptMgr& sm = ScriptMgr::singleton();
    sm.setDataFolder("scripts/");
    sm.executeScript("startup.lua");

    int screenWidth = sm.getGlobalInt("screen_width");
    int screenHeight = sm.getGlobalInt("screen_height");
    bool fullScreen = sm.getGlobalBool("screen_full");

    Engine::init("Marian", screenWidth, screenHeight, fullScreen);
    Game *game = new Game;

    Engine::singleton().mainLoop(game);

    delete game;
    Engine::shutdown();

    delete ScriptMgr::singletonPtr();

    return 0;
}
