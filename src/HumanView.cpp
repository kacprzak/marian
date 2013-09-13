/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "HumanView.h"

#include "Logger.h"
#include "Util.h"
#include "gui/GuiMgr.h"

#include <cmath> // floor
#include <vector>

// Scene scale
#define SCALE 32
// Retro style pixel perfect rendering
#define ROUND 1

HumanView::HumanView(const std::string &title, int screenWidth, int screenHeight, bool screenFull)
    : m_titile(title)
    , m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_screenFull(screenFull)
    , m_window(nullptr)
    , m_glContext(nullptr)
    , m_translate_x(0.0f)
    , m_translate_y(0.0f)
    , m_translate_z(0.0f)
    , m_scale(SCALE)
{
    try {
        initializeSDL();
    } catch (EngineError e) {
        SDL_Quit();
        throw e;
    }

    new GuiMgr;
}

//------------------------------------------------------------------------------

HumanView::~HumanView()
{
    delete GuiMgr::singletonPtr();
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
}

//------------------------------------------------------------------------------

bool HumanView::processInput(const SDL_Event &event)
{
    // Inject to gui
    return GuiMgr::singleton().processInput(event);
}

//------------------------------------------------------------------------------

void HumanView::update(float elapsedTime)
{
    // Update gui
    if (GuiMgr::singletonPtr())
        GuiMgr::singleton().update(elapsedTime);
}

//------------------------------------------------------------------------------

void HumanView::initializeSDL()
{
    int screen_flags = SDL_WINDOW_OPENGL;

    if (m_screenFull)
        screen_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    // Screen surface
    m_window = SDL_CreateWindow(m_titile.c_str(),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_screenWidth, m_screenHeight,
                                screen_flags);

    if (!m_window) {
        throw EngineError("Creating window failed", SDL_GetError());
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if(!m_glContext) {
        throw EngineError("Creating OpenGL context failed", SDL_GetError());
    }

    //SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //if (!renderer) {
    //    throw EngineError("Creating renderer failed", SDL_GetError());
    //}

    // OpenGL setup
    initializeOpenGL();
}

//------------------------------------------------------------------------------

void HumanView::initializeOpenGL()
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

void HumanView::centerViewOn(float x, float y)
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

void HumanView::viewBounds(ViewRect *rect)
{
    rect->left    = (-m_translate_x - m_screenWidth  / 2) / m_scale;
    rect->right   = (-m_translate_x + m_screenWidth  / 2) / m_scale;
    rect->bottom  = (-m_translate_y - m_screenHeight / 2) / m_scale;
    rect->top     = (-m_translate_y + m_screenHeight / 2) / m_scale;
}

//------------------------------------------------------------------------------

void HumanView::setBackgroundColor(int r, int g, int b)
{
    glClearColor(r/255.0f, g/255.0f, b/255.0f, 0);
}

//------------------------------------------------------------------------------

void HumanView::setBackgroundColor(const std::string& bgColor)
{
    if (!bgColor.empty()) {
        std::vector<int> color = hexColorToRgb(bgColor);
        setBackgroundColor(color[0], color[1], color[2]);
    }
}

//------------------------------------------------------------------------------

void HumanView::preDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(m_translate_x,
                 m_translate_y,
                 m_translate_z);
    glScalef(m_scale, m_scale, 1.0f);
}

//------------------------------------------------------------------------------

void HumanView::postDraw()
{
    // Draw debug information
    Engine::singleton().game()->drawDebugData();

    // Draw gui
    if (GuiMgr::singletonPtr())
        GuiMgr::singleton().draw();

    SDL_GL_SwapWindow(m_window);
}
