/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include <iostream>

#include "ScriptMgr.h"
#include "EventManager.h"
#include "Engine.h"
#include "Game.h"

void eventListener(EventPtr event)
{
    std::cout << "EVENT: " << event->eventName() << std::endl;
}

int main(int /*argc*/, char * /*argv*/[])
{
    new ScriptMgr;
    ScriptMgr& sm = ScriptMgr::singleton();
    sm.setDataFolder("scripts/");
    sm.executeFile("startup.lua");

    int screenWidth  = sm.getGlobalInt("screen_width");
    int screenHeight = sm.getGlobalInt("screen_height");
    bool fullScreen  = sm.getGlobalBool("screen_full");

    EventManager *em = new EventManager;
    em->addListener(GAME_STARTED, eventListener);
    em->queueEvent(EventPtr(new BaseEvent(GAME_STARTED)));

    Engine::init("Marian", screenWidth, screenHeight, fullScreen);
    Game *game = new Game;

    Engine::singleton().mainLoop(game);

    em->update();
    delete EventManager::singletonPtr();

    delete game;
    Engine::shutdown();

    delete ScriptMgr::singletonPtr();

    return 0;
}
