/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include "Logger.h"
#include "ScriptMgr.h"
#include "events/EventMgr.h"
#include "Engine.h"
#include "Game.h"
#include "graphics/HeroHumanView.h"
#include "ResourceMgr.h"

void eventListener(EventPtr event)
{
    LOG << "EVENT: " << event->eventName() << " {";
    event->serialize(LOG);
    LOG << " }"<< std::endl;
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

    new Engine;

    Game *game = new Game;

    game->attachView(std::shared_ptr<GameView>(new HeroHumanView("Marian", screenWidth, screenHeight, fullScreen)));

    Engine::singleton().mainLoop(game);

    delete game;

    delete Engine::singletonPtr();

    delete ResourceMgr::singletonPtr();
    delete EventMgr::singletonPtr();
    delete ScriptMgr::singletonPtr();

    return 0;
}
