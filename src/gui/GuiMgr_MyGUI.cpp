/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "GuiMgr.h"

#include "ResourceMgr.h"
#include "Logger.h"

#include "config.h"

#include <SDL_image.h>

#include <MyGUI.h>
#include <MyGUI_OpenGLPlatform.h>

#include <iostream>

using namespace gui;

MyGUI::MouseButton::Enum sdlMouseButtonToMyGui(const Uint8 button);

//==============================================================================

class ImageLoader : public MyGUI::OpenGLImageLoader
{
    // OpenGLImageLoader interface
public:
    void *loadImage(int &_width, int &_height, MyGUI::PixelFormat &_format, const std::string &_filename);
    void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void *_texture, const std::string &_filename);
};

//==============================================================================

static MyGUI::OpenGLPlatform *s_platform;
static MyGUI::Gui *s_gui;
static ImageLoader s_imageLoader;

//------------------------------------------------------------------------------

GuiMgr::GuiMgr()
{
    const std::string assetsFolder = ResourceMgr::singleton().dataFolder() + "MyGUI_Media";

    s_platform = new MyGUI::OpenGLPlatform();
    s_platform->initialise(&s_imageLoader);
    s_platform->getDataManagerPtr()->addResourceLocation(assetsFolder, false);

    s_gui = new MyGUI::Gui();
    s_gui->initialise();

    // Test
    MyGUI::ButtonPtr button = s_gui->createWidget<MyGUI::Button>("Button", 100, 10, 100, 26, MyGUI::Align::Default, "Main", "test");
    button->setCaption("Test");
}

//------------------------------------------------------------------------------

GuiMgr::~GuiMgr()
{
    s_gui->shutdown();
    s_platform->shutdown();

    delete s_gui;
    delete s_platform;

    LOG << "destroyed GuiMgr\n";
}

//------------------------------------------------------------------------------

bool GuiMgr::processInput(const SDL_Event& e)
{
    bool interceptEvent = false;
    MyGUI::InputManager& inputMgr = MyGUI::InputManager::getInstance();

    switch (e.type) {
        /* mouse motion handler */
    case SDL_MOUSEMOTION:
        /* we inject the mouse position directly. */
        inputMgr.injectMouseMove(e.motion.x, e.motion.y, 0);

        break;

        /* mouse down handler */
    case SDL_MOUSEBUTTONDOWN:
        /* let a special function handle the mouse button down event */
        //handle_mouse_down(e.button.button);
        inputMgr.injectMousePress(e.button.x, e.button.y, sdlMouseButtonToMyGui(e.button.button));
        break;

        /* mouse up handler */
    case SDL_MOUSEBUTTONUP:
        /* let a special function handle the mouse button up event */
        inputMgr.injectMouseRelease(e.button.x, e.button.y, sdlMouseButtonToMyGui(e.button.button));
        break;

    case SDL_MOUSEWHEEL:
        handle_mouse_wheel(e.wheel);
        break;

        /* key down */
    case SDL_KEYDOWN:
        break;

        /* key up */
    case SDL_KEYUP:
        break;

    case SDL_TEXTINPUT:
        break;

    case SDL_TEXTEDITING:
        break;

    case SDL_WINDOWEVENT:
        {
            switch (e.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                setViewSize(e.window.data1, e.window.data2);
                break;
            }

            break;
        }
    }

    return interceptEvent; // was intercepted
}

//------------------------------------------------------------------------------

void GuiMgr::update(float elapsedTime)
{

}

//------------------------------------------------------------------------------

void GuiMgr::draw()
{
    s_platform->getRenderManagerPtr()->drawOneFrame();
}

//------------------------------------------------------------------------------

void GuiMgr::setViewSize(int width, int height)
{
    s_platform->getRenderManagerPtr()->setViewSize(width, height);
}

//------------------------------------------------------------------------------

void GuiMgr::handle_mouse_down(Uint8 button)
{

}

//------------------------------------------------------------------------------

void GuiMgr::handle_mouse_wheel(const SDL_MouseWheelEvent &e)
{
    MyGUI::InputManager& inputMgr = MyGUI::InputManager::getInstance();
}

//------------------------------------------------------------------------------
 
void GuiMgr::handle_mouse_up(Uint8 button)
{
    MyGUI::InputManager& inputMgr = MyGUI::InputManager::getInstance();
}

//==============================================================================

void *ImageLoader::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
{
    std::string filename = s_platform->getDataManagerPtr()->getDataPath(_filename);

    SDL_Surface *surface = IMG_Load(filename.c_str());

    if (!surface) {
        std::cerr << "Could not load " << _filename << std::endl;
        return 0;
    }

    // work out what format to tell glTexImage2D to use...
    if (surface->format->BytesPerPixel == 3) { // RGB 24bit
        _format = MyGUI::PixelFormat::R8G8B8;
    } else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
        _format = MyGUI::PixelFormat::R8G8B8A8;
    } else {
        // Convert to 32 bits.
        SDL_PixelFormat fmt;
        memset(&fmt, 0, sizeof(fmt));
        fmt.format = SDL_PIXELFORMAT_RGBA8888;
        fmt.BitsPerPixel = 32;
        fmt.BytesPerPixel = 4;
        fmt.Rmask = 0xff;
        fmt.Gmask = 0xff00;
        fmt.Bmask = 0xff0000;
        fmt.Amask = 0xff000000;

        SDL_Surface *nimg = SDL_ConvertSurface(surface, &fmt, 0);
        SDL_FreeSurface(surface);

        if(!nimg) {
            std::cerr << "Could not convert image " << _filename << " to 32-bit\n";
            return 0;
        }

        // Done converting.
        surface = nimg;
        _format = MyGUI::PixelFormat::R8G8B8A8;
    }

    _width = surface->w;
    _height = surface->h;

    std::size_t data_size = surface->w * surface->h * surface->format->BytesPerPixel;
    unsigned char *data = new unsigned char[data_size];

    // Copy data for SDL to data
    memcpy(data, surface->pixels, data_size);

    // Clean up
    SDL_FreeSurface(surface);

    return data;
}

//------------------------------------------------------------------------------

void ImageLoader::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void *_texture, const std::string &_filename)
{

}

//==============================================================================

MyGUI::MouseButton::Enum sdlMouseButtonToMyGui(const Uint8 button)
{
    switch (button) {
    case SDL_BUTTON_LEFT:
        return MyGUI::MouseButton::Left;
    case SDL_BUTTON_MIDDLE:
        return MyGUI::MouseButton::Middle;
    case SDL_BUTTON_RIGHT:
        return MyGUI::MouseButton::Right;
    default:
        return MyGUI::MouseButton::None;
    }
}
