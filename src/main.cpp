#include "network/BaseSocketMgr.h"
#include "network/ClientSocketMgr.h"
#include "network/GameServerListenNetSocket.h"
#include "network/RemoteGameLogic.h"
#include "network/RemoteGameView.h"

#include "graphics/HeroHumanView.h"

#include "events/EventMgr.h"

#include "Engine.h"
#include "GameLogicImpl.h"
#include "ResourceMgr.h"
#include "ScriptMgr.h"

#include "Logger.h"
#include "config.h"

#include <string>

void runSingleplayer();
void runMultiplayerClient(const std::string& serverAddress);
void runMultiplayerServer();

void remoteClientEventListener(event::Event& event);

#if PLATFORM == PLATFORM_UNIX
#include <signal.h>

void ctrl_c_handler(int sgn);
void register_ctrl_c_handler();
#endif

//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    using namespace event;

    try {
        auto scriptMgr   = std::make_unique<ScriptMgr>();
        auto eventMgr    = std::make_unique<EventMgr>();
        auto resourceMgr = std::make_unique<ResourceMgr>();

        scriptMgr->executeFile("startup.lua");

        const char* scriptsFolder =
            scriptMgr->getGlobalString("scripts_folder");
        scriptMgr->setDataFolder(scriptsFolder);
        const char* assetsFolder = scriptMgr->getGlobalString("assets_folder");
        resourceMgr->setDataFolder(assetsFolder);

        if (argc >= 2 && strcmp(argv[1], "-s") == 0) {
            runMultiplayerServer();
        } else if (argc >= 2 && strcmp(argv[1], "-c") == 0) {
            std::string gameServer = "localhost";
            if (argc >= 3)
                gameServer = argv[2];
            runMultiplayerClient(gameServer);
        } else {
            runSingleplayer();
        }
    } catch (const std::exception& e) {
        Engine::showErrorMessageBox(e.what());
        LOG_FATAL << e.what() << std::endl;
        throw;
    }

    return 0;
}

//------------------------------------------------------------------------------

void runSingleplayer()
{
    int screenWidth  = ScriptMgr::singleton().getGlobalInt("screen_width");
    int screenHeight = ScriptMgr::singleton().getGlobalInt("screen_height");
    bool fullScreen  = ScriptMgr::singleton().getGlobalBool("screen_full");

    Engine engine;
    GameLogicImpl game;
    game.attachView(std::make_shared<gfx::HeroHumanView>(
        "Marian", screenWidth, screenHeight, fullScreen));
    engine.mainLoop(&game);
}

//------------------------------------------------------------------------------

void runMultiplayerClient(const std::string& serverAddress)
{
    using namespace net;

    int screenWidth  = ScriptMgr::singleton().getGlobalInt("screen_width");
    int screenHeight = ScriptMgr::singleton().getGlobalInt("screen_height");
    bool fullScreen  = ScriptMgr::singleton().getGlobalBool("screen_full");

    ClientSocketMgr* bsm = new ClientSocketMgr(serverAddress, GAME_PORT);
    int socketId         = bsm->connect();
    if (socketId == -1) {
        LOG << "Unable to connect to: " << serverAddress << std::endl;
    }

    Engine engine;
    RemoteGameLogic game(socketId);
    game.attachView(std::make_shared<gfx::HeroHumanView>(
        "Marian Cli", screenWidth, screenHeight, fullScreen));
    engine.mainLoop(&game);
}

//------------------------------------------------------------------------------

void runMultiplayerServer()
{
    using namespace net;
    using namespace event;

#if PLATFORM == PLATFORM_UNIX
    register_ctrl_c_handler();
#endif
    EventMgr& evtMgr = EventMgr::singleton();
    evtMgr.addListener(REMOTE_CLIENT, std::make_shared<EventListener>(
                                          remoteClientEventListener));

    BaseSocketMgr* bsm               = new BaseSocketMgr;
    GameServerListenNetSocket* gslns = new GameServerListenNetSocket(GAME_PORT);
    bsm->addSocket(gslns);

    Engine engine{false}; // Starts engine without video subsystem
    GameLogicImpl game;
    engine.mainLoop(&game);
}

//------------------------------------------------------------------------------

void remoteClientEventListener(event::Event& event)
{
    LOG << "EVENT: " << event.eventName() << " {" << event << " }\n";

    event::RemoteClientEvent& e = static_cast<event::RemoteClientEvent&>(event);

    std::shared_ptr<GameView> view(new net::RemoteGameView(e.m_socketId));
    Engine::singleton().game()->attachView(view);
}

//------------------------------------------------------------------------------

#if PLATFORM == PLATFORM_UNIX

void ctrl_c_handler(int sgn)
{
    LOG << "Caught signal " << sgn << std::endl;
    Engine* e = Engine::singletonPtr();
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
