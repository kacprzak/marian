/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Engine.h"

#include <iostream>
#include <cstdlib> // exit
#include <cmath> // floor
#include <SDL.h>

#include "Texture.h"
#include <vector>
#include "ResourceMgr.h"

#define SCALE 1
#define ROUND 1

Engine::Engine(const std::string& title, int screenWidth, int screenHeight)
    : m_titile(title)
    , m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
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
    } catch (SdlError e) {
        SDL_Quit();
        throw e;
    }

    // Mark all keys as not pressed
    for (int i = 0; i < SDLK_LAST; ++i) {
        m_keys[i] = false;
    }

    initializeWorld();
}

//------------------------------------------------------------------------------

Engine::~Engine()
{
    delete m_world;
    delete m_debugDraw;

    // Release all resources
    ResourceMgr::instance().release();

    std::cout << "Quitting SDL...\n";
    SDL_Quit();
}

//------------------------------------------------------------------------------

void Engine::initializeWorld()
{
    b2Vec2 gravity(0.0f, -10.0f);
    m_world = new b2World(gravity);
    m_world->SetAllowSleeping(true);

    // Debug drawing
    m_debugDraw = new DebugDraw;
    m_drawDebugData = false;

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body *groundBody = m_world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    // TODO: Continue here
}

//------------------------------------------------------------------------------

void Engine::toggleDrawDebug()
{
    if (m_drawDebugData == false) {
        m_debugDraw->SetFlags(b2Draw::e_shapeBit);
        m_world->SetDebugDraw(m_debugDraw);    
    } else {
        m_debugDraw->ClearFlags(b2Draw::e_shapeBit);
        m_world->SetDebugDraw(nullptr);
    }

    m_drawDebugData = !m_drawDebugData;
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
        if (delta > 0.0f)
            update(delta);
        draw();
    
        curr_time = SDL_GetTicks();
        delta = (float)(curr_time - last_time) / 1000.0f;
        last_time = curr_time;
    }

    m_game->clear(this);
}

//------------------------------------------------------------------------------

void Engine::centerViewOn(float x, float y)
{
#if ROUND
    m_translate_x = std::round(-x) * m_scale;
    m_translate_y = std::round(-y) * m_scale;
#else
    m_translate_x = -x * m_scale;
    m_translate_y = -y * m_scale;
#endif
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
            if (event.key.keysym.sym == SDLK_g)
                toggleDrawDebug();
            return m_game->processInput(event);
        case SDL_QUIT:
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------

void Engine::update(float elapsedTime)
{
    // Update Physics
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    m_world->Step(elapsedTime, velocityIterations, positionIterations);

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
    if (m_drawDebugData) {
        m_world->DrawDebugData();
        glColor4f(1,1,1,1); // Reset color
    }

    SDL_GL_SwapBuffers(); 
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) const
{
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y + h);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + h);
    glEnd();
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                      const GLfloat *texCoords) const
{
    glBegin(GL_QUADS);
    glTexCoord2f(texCoords[0], texCoords[1]); glVertex2f(x, y);
    glTexCoord2f(texCoords[2], texCoords[3]); glVertex2f(x + w, y);
    glTexCoord2f(texCoords[4], texCoords[5]); glVertex2f(x + w, y + h);
    glTexCoord2f(texCoords[6], texCoords[7]); glVertex2f(x, y + h);
    glEnd();
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                      GLuint texture_id, const GLfloat *texCoords) const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Enable texturing if needed.
    bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
    if(!texturing_enabled)
        glEnable(GL_TEXTURE_2D);

    // Bind texture and draw.
    glBindTexture(GL_TEXTURE_2D, texture_id);
  
    drawQuad(x, y, w, h, texCoords);

    // Disable if was disable.
    if(!texturing_enabled)
        glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
}

//------------------------------------------------------------------------------

void Engine::drawSprite(const Sprite& sprite) const
{
    const Texture *tex = sprite.texture();
    const GLfloat *texCoords = sprite.getTextureCoords();

#if ROUND
    drawQuad(std::round(sprite.position().x),
             std::round(sprite.position().y),
             sprite.width(),
             sprite.height(),
             tex->textureId(),
             texCoords);
#else
    drawQuad(sprite.position().x,
             sprite.position().y,
             sprite.width(),
             sprite.height(),
             tex->textureId(),
             texCoords);
#endif
}

//------------------------------------------------------------------------------

void Engine::initializeSDL()
{
    std::cout << "Initializing SDL...\n";

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SdlError("Could not initialize SDL", SDL_GetError());  
    }

    /* Some video inforamtion */
    const SDL_VideoInfo *info = SDL_GetVideoInfo();
  
    if (!info) {
        throw SdlError("Video query failed", SDL_GetError());
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

    std::cout << "Screen: " << m_screenWidth << "x" << m_screenHeight
              << "x" << screen_bpp << "\n";
    // Screen surface
    SDL_Surface *screen = SDL_SetVideoMode(m_screenWidth, m_screenHeight,
                                           screen_bpp, screen_flags);

    if (!screen) {
        throw SdlError("Setting video mode failed", SDL_GetError());
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

    //int r = 102, g = 140, b = 255; // blue
    int r = 0, g = 0, b = 0; // black
    glClearColor(r/255.0f, g/255.0f, b/255.0f, 0);

    //glShadeModel(GL_SMOOTH); // smooth is default
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0, ratio, 1.0, 1024.0);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glOrtho(-m_screenWidth/2, m_screenWidth/2, -m_screenHeight/2, m_screenHeight/2, -1, 1);

    glDisable(GL_DEPTH_TEST); // uncomment this if going 2D
}
