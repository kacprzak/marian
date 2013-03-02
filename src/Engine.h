/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ENGINE_H
#define ENGINE_H

#if _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <exception>
#include "Playable.h"
#include "Image.h"

class SdlError : public std::exception
{
 public:
    SdlError(const std::string& msg, char *sdlError)
        : m_msg(msg)
        , m_sdlError(sdlError)
    {
        //
    }

    ~SdlError() throw() {}

#if _MSV_VER
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
    Engine(const std::string& title, int screenWidth, int screenHeight);
    ~Engine();

    void mainLoop(Playable *game);

    void drawImage(float x, float y, float w, float h, const Image& image) const;
    void drawImage(float x, float y, float w, float h, float rotation,
                   const Image& image) const;
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

    float m_translate_x;
    float m_translate_y;
    float m_translate_z;
    float m_scale;

    Playable *m_game;

    bool m_keys[SDLK_LAST];
};

#endif
