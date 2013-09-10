/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Logger.h"
#include "ScriptMgr.h"
#include "events/EventMgr.h"
#include "Engine.h"
#include "Game.h"
#include "HumanView.h"
#include "ResourceMgr.h"
#include "network/BaseSocketManager.h"
#include "network/GameServerListenNetSocket.h"
#include "network/RemoteGameView.h"

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
    EventMgr::singleton().addListener(REMOTE_CLIENT, EventListenerPtr(new EventListener(eventListener)));

    BaseSocketManager *bsm = new BaseSocketManager;
    bsm->init();
    GameServerListenNetSocket *gslns = new GameServerListenNetSocket(GAME_PORT);
    bsm->addSocket(gslns);

    Engine::init("Marian Srv", screenWidth, screenHeight, fullScreen);

    std::shared_ptr<GameView> view(new HumanView);
    Game *game = new Game;
    game->attachView(view);

    Engine::singleton().mainLoop(game);

    delete game;
    view.reset(); // Check this!
    Engine::shutdown();

    delete BaseSocketManager::singletonPtr();
    delete ResourceMgr::singletonPtr();
    delete EventMgr::singletonPtr();
    delete ScriptMgr::singletonPtr();

    return 0;
}
