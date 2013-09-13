/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Engine.h"

#include "graphics/Texture.h"
#include "ResourceMgr.h"
#include "Logger.h"
#include "network/BaseSocketMgr.h"

#include <cstdlib> // exit

const double PI   = 3.141592653589793238462;
const float  PI_F = 3.14159265358979f;

// Maximum delta value passed to update 1/25 [s]
#define DELTA_MAX 0.04f

Engine::Engine(bool initVideo)
    : breakLoop(false)
    , m_initVideo(initVideo)
    , m_appActive(true)
    , m_mouseFocus(true)
    , m_inputFocus(true)
    , m_game(0)
{
    try {
        initializeSDL();
    } catch (EngineError e) {
        SDL_Quit();
        throw e;
    }

    new ResourceMgr;
    ResourceMgr::singleton().setDataFolder("media/");
}

//------------------------------------------------------------------------------

Engine::~Engine()
{
    // Release all resources
    delete ResourceMgr::singletonPtr();

    LOG << "Quitting SDL...\n";
    SDL_Quit();
}

//------------------------------------------------------------------------------

void Engine::mainLoop(GameLogic *game)
{
    m_game = game;
    m_game->initialize(this);
 
    unsigned int curr_time = SDL_GetTicks();
    unsigned int last_time = curr_time;
    float delta = 0.0f;

    for (;;) {
        if (breakLoop || !processEvents())
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

    m_game->cleanup(this);
}

//------------------------------------------------------------------------------

/** Return true if should keep going. */
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
    BaseSocketMgr *sm = BaseSocketMgr::singletonPtr();
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
        gv->draw(this);
    }
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

    LOG << "SDL initialized.\n";
}

