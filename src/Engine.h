/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ENGINE_H
#define ENGINE_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include "Singleton.h"
#include "GameLogic.h"
#include "graphics/Image.h"

#include <SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

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

struct ViewRect
{
    float left, right, top, bottom;
};

//------------------------------------------------------------------------------

class Engine : public Singleton<Engine>
{
 public:
    Engine(const std::string& title, int screenWidth, int screenHeight,
           bool screenFull = false);
    ~Engine();

    void mainLoop(GameLogic *game);
    GameLogic *game() { return m_game; }

    void drawImage(const Image& image, float x, float y, float rotation = 0.0f) const;
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                  GLuint textureId, const GLfloat *texCoords) const;

    void centerViewOn(float x, float y);
    void viewBounds(ViewRect *rect);

    void setBackgroundColor(int r, int g, int b);

    /** Screen size in pixels */
    int screenWidth() const  { return m_screenWidth; }
    int screenHeight() const { return m_screenHeight; }

    bool breakLoop;

 private:
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) const;
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                  const GLfloat *texCoords) const;
    void initializeSDL();
    void initializeOpenGL();

    bool processEvents();
    void update(float elapsedTime);
    void draw();

    std::string m_titile;
    int m_screenWidth;
    int m_screenHeight;
    bool m_screenFull;

    SDL_Window *m_window;

    bool m_appActive;
    bool m_mouseFocus;
    bool m_inputFocus;

    float m_translate_x;
    float m_translate_y;
    float m_translate_z;
    float m_scale;

    GameLogic *m_game;
};

#endif
