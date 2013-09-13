/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Logger.h"
#include "ScriptMgr.h"
#include "events/EventMgr.h"
#include "Engine.h"
#include "Game.h"
#include "HeroHumanView.h"
#include "ResourceMgr.h"
#include "network/ClientSocketMgr.h"
#include "network/GameServerListenNetSocket.h"
#include "network/RemoteGameLogic.h"

void eventListener(EventPtr event)
{
    LOG << "EVENT: " << event->eventName() << " {";
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
    //EventMgr::singleton().addListener(REMOTE_CLIENT, EventListenerPtr(new EventListener(eventListener)));

    const char *gameServer = "localhost";
    ClientSocketMgr *bsm = new ClientSocketMgr(gameServer, GAME_PORT);
    bsm->init();
    int socketId = bsm->connect();
    if (socketId == -1) {
        LOG << "Unable to connect to: " << gameServer << std::endl;
    }

    new Engine("Marian Cli", screenWidth, screenHeight, fullScreen);

    GameLogic *game = new RemoteGameLogic(socketId);

    game->attachView(std::shared_ptr<GameView>(new HumanView));

    Engine::singleton().mainLoop(game);

    delete game;

    delete Engine::singletonPtr();

    delete ClientSocketMgr::singletonPtr();
    delete ResourceMgr::singletonPtr();
    delete EventMgr::singletonPtr();
    delete ScriptMgr::singletonPtr();

    return 0;
}
