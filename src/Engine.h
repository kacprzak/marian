/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ENGINE_H
#define ENGINE_H

#include "config.h"

#if PLATFORM == PLATFORM_WINDOWS
  #include <windows.h>
#endif

#include "Singleton.h"
#include "GameLogic.h"

#include <SDL.h>

#include <string>
#include <stdexcept>

class EngineError : public std::runtime_error
{
 public:
    EngineError(const std::string& msg, const char *sdlError)
        : std::runtime_error(msg + ": " + sdlError)
        , m_sdlError(sdlError)
    {}

    const char *sdlError() { return m_sdlError; }

 private:
    const char *m_sdlError;
};

//------------------------------------------------------------------------------

/*!
 * \brief Main loop.
 */
class Engine : public Singleton<Engine>
{
 public:
    Engine(bool initVideo = true);
    ~Engine() override;

    void mainLoop(GameLogic *game);
    GameLogic *game() { return m_game; }

    void breakLoop() { m_breakLoop = true; }

    static int showErrorMessageBox(const char *msg);

 private:
    void initializeSDL();

    bool processEvents();
    void update(float elapsedTime);
    void draw();

    bool m_breakLoop;
    bool m_initVideo;

    bool m_appActive;
    bool m_mouseFocus;
    bool m_inputFocus;

    GameLogic *m_game;
};

#endif
