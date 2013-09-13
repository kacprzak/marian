/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Engine.h"

#include "graphics/Texture.h"
#include "ResourceMgr.h"
#include "gui/GuiMgr.h"
#include "Logger.h"
#include "network/BaseSocketMgr.h"

#include <vector>
#include <cstdlib> // exit
#include <cmath> // floor

const double PI   = 3.141592653589793238462;
const float  PI_F = 3.14159265358979f;

// Maximum delta value passed to update 1/25 [s]
#define DELTA_MAX 0.04f
// Scene scale
#define SCALE 32
// Retro style pixel perfect rendering
#define ROUND 1

Engine::Engine(const std::string& title, int screenWidth, int screenHeight,
               bool screenFull)
    : breakLoop(false)
    , m_titile(title)
    , m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_screenFull(screenFull)
    , m_window(nullptr)
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
    } catch (EngineError e) {
        SDL_Quit();
        throw e;
    }

    new ResourceMgr;
    ResourceMgr::singleton().setDataFolder("media/");

    // If video is initialized then init GUI system
    if (m_window)
        new GuiMgr;
}

//------------------------------------------------------------------------------

Engine::~Engine()
{
    delete GuiMgr::singletonPtr();
    // Release all resources
    delete ResourceMgr::singletonPtr();

    LOG << "Quitting SDL...\n";
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

//------------------------------------------------------------------------------

void Engine::mainLoop(GameLogic *game)
{
    m_game = game;
    m_game->initialize(this);
    for (auto gv : m_game->gameViews()) {
        gv->initialize(this);
    }
 
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

    for (auto gv : m_game->gameViews()) {
        gv->cleanup(this);
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

void Engine::viewBounds(ViewRect *rect)
{
    rect->left    = (-m_translate_x - m_screenWidth  / 2) / m_scale;
    rect->right   = (-m_translate_x + m_screenWidth  / 2) / m_scale;
    rect->bottom  = (-m_translate_y - m_screenHeight / 2) / m_scale;
    rect->top     = (-m_translate_y + m_screenHeight / 2) / m_scale;
}

//------------------------------------------------------------------------------

/** Return true if should keep going. */
bool Engine::processEvents()
{
    SDL_Event event;
  
    while (SDL_PollEvent(&event)) {
        // Inject to gui
        bool intercepted = GuiMgr::singleton().processInput(event);
        if (intercepted) continue;

        switch (event.type) {
        case SDL_KEYUP:
            for (auto gv : m_game->gameViews()) {
                gv->processInput(event);
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_G) m_game->toggleDrawDebug();
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) return false;
            for (auto gv : m_game->gameViews()) {
                gv->processInput(event);
            }
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
    }

    return true;
}

//------------------------------------------------------------------------------

void Engine::update(float elapsedTime)
{
    // Update game
    m_game->update(elapsedTime);
    // Update views
    for (auto gv : m_game->gameViews())
        gv->update(elapsedTime);
    // Update gui
    if (GuiMgr::singletonPtr())
        GuiMgr::singleton().update(elapsedTime);

    BaseSocketMgr *sm = BaseSocketMgr::singletonPtr();
    if (sm)
        sm->select(20);
}

//------------------------------------------------------------------------------

void Engine::draw()
{
    if (!m_window)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(m_translate_x,
                 m_translate_y,
                 m_translate_z);
    glScalef(m_scale, m_scale, 1.0f);

    // Draw game
    for (auto gv : m_game->gameViews()) {
        gv->draw(this);
    }

    // Draw gui
    if (GuiMgr::singletonPtr())
        GuiMgr::singleton().draw();

    // Draw debug information
    m_game->drawDebugData();

    SDL_GL_SwapWindow(m_window);
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

    if (m_screenWidth > 0 && m_screenHeight > 0) {

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw EngineError("Could not initialize SDL", SDL_GetError());
        }

        /* Some video inforamtion */
        //const SDL_VideoInfo *info = SDL_GetVideoInfo();

        //if (!info) {
        //    throw EngineError("Video query failed", SDL_GetError());
        //}

        //int screen_bpp = info->vfmt->BitsPerPixel;

        int screen_flags = SDL_WINDOW_OPENGL;

        if (m_screenFull)
            screen_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

#if 1
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetSwapInterval(1);
#endif

        // Screen surface
        m_window = SDL_CreateWindow(m_titile.c_str(),
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    m_screenWidth, m_screenHeight,
                                    screen_flags);

        if (!m_window) {
            throw EngineError("Creating window failed", SDL_GetError());
        }

        if(!SDL_GL_CreateContext(m_window)) {
            throw EngineError("Creating OpenGL context failed", SDL_GetError());
        }

        //SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        //if (!renderer) {
        //    throw EngineError("Creating renderer failed", SDL_GetError());
        //}

        //SDL_ShowCursor(SDL_DISABLE);
        //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
        SDL_StopTextInput(); // Disable text input events when GUI is not visible

        // OpenGL setup
        initializeOpenGL();

    } else {
        if (SDL_Init(0) < 0) {
            throw EngineError("Could not initialize SDL", SDL_GetError());
        }
    }

    LOG << "SDL initialized.\n";
}

//------------------------------------------------------------------------------

void Engine::initializeOpenGL()
{
    LOG << "Initializing OpenGl...\n";
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
