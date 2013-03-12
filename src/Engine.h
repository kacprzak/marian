/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ENGINE_H
#define ENGINE_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <exception>
#include "Playable.h"
#include "Image.h"

class EngineError : public std::exception
{
 public:
    EngineError(const std::string& msg, char *sdlError)
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
    char *m_sdlError;
};

//------------------------------------------------------------------------------

class Engine
{
 public:
    static Engine& instance()
    {
        return *s_instance;
    }

    static void init(const std::string& title, int screenWidth, int screenHeight,
                     bool screenFull = false);
    static void shutdown();

    void mainLoop(Playable *game);
    Playable *game() { return m_game; } 

    void drawImage(const Image& image, float x, float y, float rotation = 0.0f) const;
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                  GLuint textureId, const GLfloat *texCoords) const;

    void centerViewOn(float x, float y);
    void viewBounds(float *left, float *right, float *bottom, float *top);

    void setBackgroundColor(int r, int g, int b);

    bool isPressed(int keycode) const { return m_keys[keycode]; }

    /** Screen size in pixels */
    int screenWidth() const  { return m_screenWidth; }
    int screenHeight() const { return m_screenHeight; }

 private:
    Engine(const std::string& title, int screenWidth, int screenHeight,
           bool screenFull = false);
    ~Engine();

    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) const;
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                  const GLfloat *texCoords) const;
    void initializeSDL();
    void initializeOpenGL();

    bool processEvents();
    void update(float elapsedTime);
    void draw();

    // Singleton instance
    static Engine *s_instance;

    std::string m_titile;
    int m_screenWidth;
    int m_screenHeight;
    bool m_screenFull;

    float m_translate_x;
    float m_translate_y;
    float m_translate_z;
    float m_scale;

    Playable *m_game;

    bool m_keys[SDLK_LAST];
};

#endif
