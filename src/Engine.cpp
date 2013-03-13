/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Engine.h"

#include <iostream>
#include <cstdlib> // exit
#include <cmath> // floor
#include <SDL.h>

#include "Texture.h"
#include <vector>
#include "ResourceMgr.h"

const double PI   = 3.141592653589793238462;
const float  PI_F = 3.14159265358979f;

// Global pointer (for use in Lua exposed functions)
Engine *Engine::s_instance = nullptr;

#define SCALE 32
#define ROUND 1 // Retro style pixel perfect rendering

void Engine::init(const std::string& title, int screenWidth, int screenHeight,
                  bool screenFull)
{
    if (s_instance) {
        std::cerr << "Engine is initialized!";
        return;
    }

    s_instance = new Engine(title, screenWidth, screenHeight, screenFull);
}

//------------------------------------------------------------------------------

void Engine::shutdown()
{
    delete s_instance;
    s_instance = nullptr;
}

//------------------------------------------------------------------------------

Engine::Engine(const std::string& title, int screenWidth, int screenHeight,
               bool screenFull)
    : m_titile(title)
    , m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_screenFull(screenFull)
    , m_appActive(true)
    , m_mouseFocus(true)
    , m_inputFocus(true)
    , m_translate_x(0.0f)
    , m_translate_y(0.0f)
    , m_translate_z(0.0f)
    , m_scale(SCALE)
    , m_game(0)
{
    try {
        initializeSDL();
        initializeOpenGL();
        SDL_WM_SetCaption(title.c_str(), title.c_str());
    } catch (EngineError e) {
        SDL_Quit();
        throw e;
    }

    // Mark all keys as not pressed
    for (int i = 0; i < SDLK_LAST; ++i) {
        m_keys[i] = false;
    }
}

//------------------------------------------------------------------------------

Engine::~Engine()
{
    // Release all resources
    ResourceMgr::instance().release();

    std::cout << "Quitting SDL...\n";
    SDL_Quit();
}

//------------------------------------------------------------------------------

void Engine::mainLoop(Playable *game)
{
    m_game = game;
    m_game->initialize(this);
 
    unsigned int curr_time = SDL_GetTicks();
    unsigned int last_time = curr_time;
    float delta = 0.0f;

    for (;;) {
        if (!processEvents())
            break;
        if (m_appActive) {
            if (m_mouseFocus && m_inputFocus && delta > 0.0f)
                update(delta);
            draw();
        }
    
        curr_time = SDL_GetTicks();
        delta = float(curr_time - last_time) / 1000.0f;
        last_time = curr_time;
    }

    m_game->cleanup(this);
}

//------------------------------------------------------------------------------

void Engine::centerViewOn(float x, float y)
{
#if ROUND
    m_translate_x = std::floor(-x * m_scale + 0.5f); // No std::round in MSVC
    m_translate_y = std::floor(-y * m_scale + 0.5f);
#else
    m_translate_x = -x * m_scale;
    m_translate_y = -y * m_scale;
#endif
}

//------------------------------------------------------------------------------

void Engine::viewBounds(float *left, float *right, float *bottom, float *top)
{
    *left    = (-m_translate_x - m_screenWidth  / 2) / m_scale;
    *right   = (-m_translate_x + m_screenWidth  / 2) / m_scale;
    *bottom  = (-m_translate_y - m_screenHeight / 2) / m_scale;
    *top     = (-m_translate_y + m_screenHeight / 2) / m_scale;
}

//------------------------------------------------------------------------------

/** Return true if should keep going. */
bool Engine::processEvents()
{
    SDL_Event event;
  
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYUP:
            m_keys[event.key.keysym.sym] = false;
            return m_game->processInput(event);
        case SDL_KEYDOWN:
            m_keys[event.key.keysym.sym] = true;
            return m_game->processInput(event);
        case SDL_ACTIVEEVENT:
            switch (event.active.state)    {
            case SDL_APPACTIVE:
                m_appActive = event.active.gain;
                break;
            case SDL_APPMOUSEFOCUS:
                m_mouseFocus = event.active.gain;
                break;
            case SDL_APPINPUTFOCUS:
                m_inputFocus = event.active.gain;
                break;
            }
            break;
        case SDL_QUIT:
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------

void Engine::update(float elapsedTime)
{
    // Update world
    m_game->update(this, elapsedTime);
}

//------------------------------------------------------------------------------

void Engine::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(m_translate_x,
                 m_translate_y,
                 m_translate_z);
    glScalef(m_scale, m_scale, 1.0f);

    m_game->draw(this);

    SDL_GL_SwapBuffers(); 
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) const
{
    // Disable texturing if needed.
    GLboolean texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
    if(texturing_enabled)
        glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y + h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + h);
    }
    glEnd();

    if(texturing_enabled)
        glEnable(GL_TEXTURE_2D);
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                      const GLfloat *texCoords) const
{
    glBegin(GL_QUADS);
    {
        glTexCoord2f(texCoords[0], texCoords[1]); glVertex2f(x, y);
        glTexCoord2f(texCoords[2], texCoords[3]); glVertex2f(x + w, y);
        glTexCoord2f(texCoords[4], texCoords[5]); glVertex2f(x + w, y + h);
        glTexCoord2f(texCoords[6], texCoords[7]); glVertex2f(x, y + h);
    }
    glEnd();
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                      GLuint texture_id, const GLfloat *texCoords) const
{
    // Enable texturing if needed.
    GLboolean texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
    if(!texturing_enabled)
        glEnable(GL_TEXTURE_2D);

    // Bind texture and draw.
    glBindTexture(GL_TEXTURE_2D, texture_id);
  
    drawQuad(x, y, w, h, texCoords);

    // Disable if it was disabled.
    if(!texturing_enabled)
        glDisable(GL_TEXTURE_2D);
}

//------------------------------------------------------------------------------

void Engine::drawImage(const Image& image, float x, float y, float rotation) const
{
    float deg = rotation * 180.0f / PI_F;
    float hw = image.width()  / 2.0f;
    float hh = image.height() / 2.0f;
    
    const Texture *tex = image.texture();
    const GLfloat *texCoords = image.getTextureCoords();

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(deg, 0.0f, 0.0f, 1.0f);

    drawQuad(-hw, -hh, image.width(), image.height(), tex->textureId(), texCoords);

    glPopMatrix();
}

//------------------------------------------------------------------------------

void Engine::initializeSDL()
{
    std::cout << "Initializing SDL...\n";

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw EngineError("Could not initialize SDL", SDL_GetError());  
    }

    /* Some video inforamtion */
    const SDL_VideoInfo *info = SDL_GetVideoInfo();
  
    if (!info) {
        throw EngineError("Video query failed", SDL_GetError());
    }

    int screen_bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

    char env[] = "SDL_VIDEO_CENTERED=center";
    SDL_putenv(env);

    int screen_flags = SDL_OPENGL;

    if (m_screenFull)
        screen_flags |= SDL_FULLSCREEN;

    std::cout << "Screen: " << m_screenWidth << "x" << m_screenHeight
              << "x" << screen_bpp << "\n";
    // Screen surface
    SDL_Surface *screen = SDL_SetVideoMode(m_screenWidth, m_screenHeight,
                                           screen_bpp, screen_flags);

    if (!screen) {
        throw EngineError("Setting video mode failed", SDL_GetError());
    }

    SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);  

    std::cout << "SDL initialized.\n";
}

//------------------------------------------------------------------------------

void Engine::initializeOpenGL()
{
    std::cout << "Initializing OpenGl...\n";
    //float ratio = float(m_screenWidth) / float(m_screenHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 0); // black
    //glShadeModel(GL_SMOOTH); // smooth is default
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0, ratio, 1.0, 1024.0);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glOrtho(-m_screenWidth/2, m_screenWidth/2, -m_screenHeight/2, m_screenHeight/2, -1, 1);

    glDisable(GL_DEPTH_TEST); // uncomment this if going 2D

    glEnable(GL_TEXTURE_2D); // Enable textures by default

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

//------------------------------------------------------------------------------

void Engine::setBackgroundColor(int r, int g, int b)
{
    glClearColor(r/255.0f, g/255.0f, b/255.0f, 0);
}
