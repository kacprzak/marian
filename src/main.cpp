/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "network/BaseSocketMgr.h"
#include "network/ClientSocketMgr.h"
#include "network/RemoteGameLogic.h"
#include "network/GameServerListenNetSocket.h"
#include "network/RemoteGameView.h"

#include "graphics/HeroHumanView.h"

#include "events/EventMgr.h"

#include "ScriptMgr.h"
#include "ResourceMgr.h"
#include "Engine.h"
#include "Game.h"

#include "Logger.h"
#include "config.h"

#include <string>

void runSingleplayer();
void runMultiplayerClient(const std::string& serverAddress);
void runMultiplayerServer();

void remoteClientEventListener(EventPtr event);

#if PLATFORM == PLATFORM_UNIX
  #include <signal.h>

void ctrl_c_handler(int sgn);
void register_ctrl_c_handler();
#endif

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    try {
        new ScriptMgr;
        new EventMgr;

        ScriptMgr::singleton().setDataFolder("scripts/");
        ScriptMgr::singleton().executeFile("startup.lua");

        new ResourceMgr;
        ResourceMgr::singleton().setDataFolder("assets/");

        if (argc >= 2 && strcmp(argv[1], "-s") == 0)
        {
            runMultiplayerServer();
        }
        else if (argc >= 2 && strcmp(argv[1], "-c") == 0)
        {
            std::string gameServer = "localhost";
            if (argc >= 3)
                gameServer = argv[2];
            runMultiplayerClient(gameServer);
        }
        else
        {
            runSingleplayer();
        }

        delete ResourceMgr::singletonPtr();
        delete EventMgr::singletonPtr();
        delete ScriptMgr::singletonPtr();

    } catch (const std::exception& e) {
        LOG_FATAL << e.what() << std::endl;
        throw;
    }
}

//------------------------------------------------------------------------------

void runSingleplayer()
{
    int screenWidth  = ScriptMgr::singleton().getGlobalInt("screen_width");
    int screenHeight = ScriptMgr::singleton().getGlobalInt("screen_height");
    bool fullScreen  = ScriptMgr::singleton().getGlobalBool("screen_full");

    new Engine;
    Game *game = new Game;
    game->attachView(std::shared_ptr<GameView>(new HeroHumanView("Marian", screenWidth, screenHeight, fullScreen)));
    Engine::singleton().mainLoop(game);
    delete game;
    delete Engine::singletonPtr();
}

//------------------------------------------------------------------------------

void runMultiplayerClient(const std::string& serverAddress)
{
    int screenWidth  = ScriptMgr::singleton().getGlobalInt("screen_width");
    int screenHeight = ScriptMgr::singleton().getGlobalInt("screen_height");
    bool fullScreen  = ScriptMgr::singleton().getGlobalBool("screen_full");

    ClientSocketMgr *bsm = new ClientSocketMgr(serverAddress, GAME_PORT);
    bsm->init();
    int socketId = bsm->connect();
    if (socketId == -1) {
        LOG << "Unable to connect to: " << serverAddress << std::endl;
    }

    new Engine;
    GameLogic *game = new RemoteGameLogic(socketId);
    game->attachView(std::shared_ptr<GameView>(new HeroHumanView("Marian Cli", screenWidth, screenHeight, fullScreen)));
    Engine::singleton().mainLoop(game);
    delete game;
    delete Engine::singletonPtr();
}

//------------------------------------------------------------------------------

void runMultiplayerServer()
{
#if PLATFORM == PLATFORM_UNIX
    register_ctrl_c_handler();
#endif
    EventMgr::singleton().addListener(REMOTE_CLIENT, EventListenerPtr(new EventListener(remoteClientEventListener)));

    BaseSocketMgr *bsm = new BaseSocketMgr;
    bsm->init();
    GameServerListenNetSocket *gslns = new GameServerListenNetSocket(GAME_PORT);
    bsm->addSocket(gslns);

    new Engine(false); // Starts engine without video subsystem
    Game *game = new Game;
    Engine::singleton().mainLoop(game);
    delete game;
    delete Engine::singletonPtr();
}

//------------------------------------------------------------------------------

void remoteClientEventListener(EventPtr event)
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

#if PLATFORM == PLATFORM_UNIX

void ctrl_c_handler(int sgn)
{
    LOG << "Caught signal " << sgn << std::endl;
    Engine *e = Engine::singletonPtr();
    if (e)
        e->breakLoop();
    else
        exit(-1);
}

//------------------------------------------------------------------------------

void register_ctrl_c_handler()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = ctrl_c_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}

#endif
