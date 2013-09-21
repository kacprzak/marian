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
#include <exception>

class EngineError : public std::exception
{
 public:
    EngineError(const std::string& msg, const char *sdlError)
        : m_msg(msg)
        , m_sdlError(sdlError)
    {
        //
    }

    ~EngineError() throw() {}

#ifdef _MSC_VER
    const char* what() const //noexcept (MSVC er C3646)
#else
    const char* what() const noexcept
#endif
    {
        std::string fullMsg = m_msg + ": " + m_sdlError;
        return fullMsg.c_str();
    }

 private:
    const std::string m_msg;
    const char *m_sdlError;
};

//------------------------------------------------------------------------------

class Engine : public Singleton<Engine>
{
 public:
    Engine(bool initVideo = true);
    ~Engine();

    void mainLoop(GameLogic *game);
    GameLogic *game() { return m_game; }

    bool breakLoop;

 private:
    void initializeSDL();

    bool processEvents();
    void update(float elapsedTime);
    void draw();

    bool m_initVideo;

    bool m_appActive;
    bool m_mouseFocus;
    bool m_inputFocus;

    GameLogic *m_game;
};

#endif
