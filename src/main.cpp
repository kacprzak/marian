/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include <iostream>

#include "ScriptMgr.h"
#include "EventMgr.h"
#include "Engine.h"
#include "Game.h"
#include "HumanView.h"
#include "ResourceMgr.h"

void eventListener(EventPtr event)
{
    std::clog << "EVENT: " << event->eventName() << " {";
    event->serialize(std::clog);
    std::clog << " }"<< std::endl;
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

    new EventMgr;
    //EventMgr::singleton().addListener(ACTOR_COLLIDED, eventListener);

    Engine::init("Marian", screenWidth, screenHeight, fullScreen);

    std::shared_ptr<GameView> view(new HumanView);
    Game *game = new Game;
    game->attachView(view);

    Engine::singleton().mainLoop(game);

    delete game;
    view.reset(); // Check this!
    Engine::shutdown();

    delete ResourceMgr::singletonPtr();
    delete EventMgr::singletonPtr();
    delete ScriptMgr::singletonPtr();

    return 0;
}
