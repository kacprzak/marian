/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Logger.h"
#include "ScriptMgr.h"
#include "events/EventMgr.h"
#include "Engine.h"
#include "Game.h"
#include "graphics/HeroHumanView.h"
#include "ResourceMgr.h"
#include "network/BaseSocketMgr.h"
#include "network/GameServerListenNetSocket.h"
#include "network/RemoteGameView.h"

#include <signal.h>

void eventListener(EventPtr event)
{
    LOG << "EVENT: " << event->eventName() << " {";
    event->serialize(std::clog);
    std::clog << " }"<< std::endl;

    // New client
    auto e = std::static_pointer_cast<RemoteClientEvent>(event);

    std::shared_ptr<GameView> view(new RemoteGameView(e->m_socketId));
    Engine::singleton().game()->attachView(view);
}

//------------------------------------------------------------------------------

void ctrl_c_handler(int s){
    LOG << "Caught signal " << s << std::endl;
    Engine *e = Engine::singletonPtr();
    if (e)
        e->breakLoop = true;
    else
        exit(-1);
}

//------------------------------------------------------------------------------

void register_ctrl_c_handler() {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = ctrl_c_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}

//------------------------------------------------------------------------------

#define NO_VIDEO

int main(int /*argc*/, char * /*argv*/[])
{
    new ScriptMgr;
    ScriptMgr& sm = ScriptMgr::singleton();
    sm.setDataFolder("scripts/");

#ifndef NO_VIDEO
    sm.executeFile("startup.lua");

    int screenWidth  = sm.getGlobalInt("screen_width");
    int screenHeight = sm.getGlobalInt("screen_height");
    bool fullScreen  = sm.getGlobalBool("screen_full");
#else
    register_ctrl_c_handler();
#endif

    new EventMgr;
    EventMgr::singleton().addListener(REMOTE_CLIENT, EventListenerPtr(new EventListener(eventListener)));

    BaseSocketMgr *bsm = new BaseSocketMgr;
    bsm->init();
    GameServerListenNetSocket *gslns = new GameServerListenNetSocket(GAME_PORT);
    bsm->addSocket(gslns);

    new Engine(false);

    Game *game = new Game;

#ifndef NO_VIDEO
    game->attachView(std::shared_ptr<GameView>(new HumanView));
#endif

    Engine::singleton().mainLoop(game);

    delete game;

    delete Engine::singletonPtr();

    delete BaseSocketMgr::singletonPtr();
    delete ResourceMgr::singletonPtr();
    delete EventMgr::singletonPtr();
    delete ScriptMgr::singletonPtr();

    return 0;
}
