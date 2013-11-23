/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Engine.h"

#include "ResourceMgr.h"
#include "Logger.h"
#include "network/BaseSocketMgr.h"

#include <cstdlib> // exit

// Maximum delta value passed to update 1/25 [s]
#define DELTA_MAX 0.04f

Engine::Engine(bool initVideo)
    : m_breakLoop(false)
    , m_initVideo(initVideo)
    , m_appActive(true)
    , m_mouseFocus(true)
    , m_inputFocus(true)
    , m_game(nullptr)
{
    try {
        initializeSDL();
    } catch (const EngineError& /*e*/) {
        SDL_Quit();
        throw;
    }
}

//------------------------------------------------------------------------------

Engine::~Engine()
{
    ResourceMgr::singleton().releaseTextures(); // Before SDL is closed

    LOG << "Quitting SDL...\n";
    SDL_Quit();
}

//------------------------------------------------------------------------------

void Engine::initializeSDL()
{
    LOG << "Initializing SDL...\n";

    uint32 sdl_flags = 0;
    if (m_initVideo)
        sdl_flags |= SDL_INIT_VIDEO;

    if (SDL_Init(sdl_flags) < 0) {
        throw EngineError("Could not initialize SDL", SDL_GetError());
    }

#if 1
    if (m_initVideo) {
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetSwapInterval(1);
    }
#endif

    //SDL_ShowCursor(SDL_DISABLE);
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_StopTextInput(); // Disable text input events when GUI is not visible

    LOG << "SDL initialized\n";
}

//------------------------------------------------------------------------------

void Engine::mainLoop(GameLogic *game)
{
    m_game = game;
    m_game->onBeforeMainLoop(this);
 
    unsigned int curr_time = SDL_GetTicks();
    unsigned int last_time = curr_time;
    float delta = 0.0f;

    for (;;) {
        if (m_breakLoop || !processEvents())
            break;
        if (m_appActive) {
            if (/*m_inputFocus &&*/ delta > 0.0f) {
                while (delta > DELTA_MAX) {
                    update(DELTA_MAX);
                    delta -= DELTA_MAX;
                }
                update(delta);
            }
            draw();
        }
    
        curr_time = SDL_GetTicks();
        delta = float(curr_time - last_time) / 1000.0f;
        last_time = curr_time;
    }

    m_game->onAfterMainLoop(this);
}

//------------------------------------------------------------------------------

/*! Return true if should keep going. */
bool Engine::processEvents()
{
    SDL_Event event;
  
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYUP:
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_G) m_game->toggleDrawDebug();
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) return false;
            break;
        case SDL_WINDOWEVENT:
        {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                m_inputFocus = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                m_inputFocus = false;
                break;
            default:
                break;
            }
            break;
        }
        case SDL_QUIT:
            return false;
        }

        for (auto gv : m_game->gameViews()) {
            gv->processInput(event);
        }
    }

    return true;
}

//------------------------------------------------------------------------------

void Engine::update(float elapsedTime)
{
    // Update game logic
    m_game->update(elapsedTime);

    // Update views
    for (auto gv : m_game->gameViews())
        gv->update(elapsedTime);

    // Update network
    net::BaseSocketMgr *sm = net::BaseSocketMgr::singletonPtr();
    if (sm)
        sm->select(20);
}

//------------------------------------------------------------------------------

void Engine::draw()
{
    if (!m_initVideo)
        return;

    // Draw game
    for (auto gv : m_game->gameViews()) {
        gv->draw();
    }
}

//------------------------------------------------------------------------------

int Engine::showErrorMessageBox(const char *msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg, NULL);
}
