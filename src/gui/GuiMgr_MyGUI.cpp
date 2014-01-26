/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "GuiMgr.h"

#define MYGUI_DONT_REPLACE_NULLPTR

#include "Console_MyGUI.h"

#include "ResourceMgr.h"
#include "Logger.h"

#include "config.h"

#include <SDL_image.h>

#include <MyGUI.h>
#include <MyGUI_OpenGLPlatform.h>

#include <iostream>

using namespace gui;

MyGUI::MouseButton::Enum SDLMouseButtonToMyGUI(const Uint8 button);
MyGUI::Char SDLKeycodeToMyGUI(SDL_Keycode code);
MyGUI::KeyCode SDLScancodeToMyGUI(SDL_Scancode code);

//==============================================================================

class ImageLoader : public MyGUI::OpenGLImageLoader
{
    // OpenGLImageLoader interface
public:
    void *loadImage(int &_width, int &_height, MyGUI::PixelFormat &_format, const std::string &_filename);
    void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void *_texture, const std::string &_filename);
};

//==============================================================================

static MyGUI::OpenGLPlatform *s_platform = nullptr;
static MyGUI::Gui *s_gui = nullptr;
static ImageLoader s_imageLoader;

static Console *s_console = nullptr;

//------------------------------------------------------------------------------

GuiMgr::GuiMgr()
{
    const std::string assetsFolder = ResourceMgr::singleton().dataFolder() + "MyGUI_Media";

    s_platform = new MyGUI::OpenGLPlatform;

    MyGUI::LogManager::getInstance().setSTDOutputEnabled(false);

    s_platform->initialise(&s_imageLoader);
    s_platform->getDataManagerPtr()->addResourceLocation(assetsFolder, false);

    s_gui = new MyGUI::Gui;
    s_gui->initialise();

    s_console = new Console;
    s_console->setVisible(false);

    LOG << "created GuiMgr\n";
}

//------------------------------------------------------------------------------

GuiMgr::~GuiMgr()
{
    delete s_console;

    s_gui->shutdown();
    delete s_gui;

    s_platform->shutdown();
    delete s_platform;

    LOG << "destroyed GuiMgr\n";
}

//------------------------------------------------------------------------------

bool GuiMgr::processInput(const SDL_Event& e)
{
    bool interceptEvent = false;
    MyGUI::InputManager& inputMgr = MyGUI::InputManager::getInstance();

    switch (e.type) {

    case SDL_MOUSEMOTION:
        inputMgr.injectMouseMove(e.motion.x, e.motion.y, 0);
        break;

    case SDL_MOUSEBUTTONDOWN:
        inputMgr.injectMousePress(e.button.x, e.button.y, SDLMouseButtonToMyGUI(e.button.button));
        break;

    case SDL_MOUSEBUTTONUP:
        inputMgr.injectMouseRelease(e.button.x, e.button.y, SDLMouseButtonToMyGUI(e.button.button));
        break;

    case SDL_MOUSEWHEEL:
        //handle_mouse_wheel(e.wheel);
        break;

    case SDL_KEYDOWN:
        {
            MyGUI::KeyCode key = SDLScancodeToMyGUI(e.key.keysym.scancode);

            bool toogleConsole = (e.key.keysym.sym == SDLK_BACKQUOTE);

            if (s_console) {
                if (s_console->isVisible()) s_console->handleKey(key);

                if (toogleConsole && !s_console->isVisible()) {
                    SDL_StartTextInput();
                    s_console->setVisible(true);
                } else if (toogleConsole && s_console->isVisible()) {
                    s_console->setVisible(false);
                    SDL_StopTextInput();
                }
            }

            inputMgr.injectKeyPress(key);
        }
        break;

    case SDL_KEYUP:
        inputMgr.injectKeyRelease(SDLScancodeToMyGUI(e.key.keysym.scancode));
        break;

    case SDL_TEXTINPUT:
        {
            const char *ch = e.text.text;
            while (*ch != '\0') {
                inputMgr.injectKeyPress(MyGUI::KeyCode::None, *ch);
                ch++;
            }
        }
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

    if (s_console->isVisible())
        interceptEvent = true;

    return interceptEvent; // was intercepted
}

//------------------------------------------------------------------------------

void GuiMgr::update(float /* elapsedTime */)
{
    // MyGUI manages time on it's own
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

}

//------------------------------------------------------------------------------
 
void GuiMgr::handle_mouse_up(Uint8 button)
{

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
    Uint8 *data = new Uint8[data_size];

    // Copy data for SDL to data
    SDL_PixelFormat *fmt = surface->format;
    SDL_LockSurface(surface);

    if (_format == MyGUI::PixelFormat::R8G8B8) {
        memcpy(data, surface->pixels, data_size);
    } else { // RGBA
        Uint32 *pixels = (Uint32*)surface->pixels;
        int pixels_size = surface->w * surface->h;
        for (int i = 0; i < pixels_size; ++i) {
            Uint32 pixel = pixels[i];
            Uint32 temp;
            Uint8 red, green, blue, alpha;

            /* Get Red component */
            temp = pixel & fmt->Rmask;  /* Isolate red component */
            temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
            temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
            red = (Uint8)temp;

            /* Get Green component */
            temp = pixel & fmt->Gmask;  /* Isolate green component */
            temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
            temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
            green = (Uint8)temp;

            /* Get Blue component */
            temp = pixel & fmt->Bmask;  /* Isolate blue component */
            temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
            temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
            blue = (Uint8)temp;

            /* Get Alpha component */
            temp = pixel & fmt->Amask;  /* Isolate alpha component */
            temp = temp >> fmt->Ashift; /* Shift it down to 8-bit */
            temp = temp << fmt->Aloss;  /* Expand to a full 8-bit number */
            alpha = (Uint8)temp;

            int data_idx = i * 4;
            // This (BGRA) works but why?
            data[data_idx + 2] = red;
            data[data_idx + 1] = green;
            data[data_idx + 0] = blue;
            data[data_idx + 3] = alpha;
        }
    }

    SDL_UnlockSurface(surface);
    // Clean up
    SDL_FreeSurface(surface);

    return data;
}

//------------------------------------------------------------------------------

void ImageLoader::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void *_texture, const std::string &_filename)
{
    throw std::logic_error("Not implemented!");
}

//==============================================================================

MyGUI::MouseButton::Enum SDLMouseButtonToMyGUI(const Uint8 button)
{
    switch (button) {
    case SDL_BUTTON_LEFT:   return MyGUI::MouseButton::Left;
    case SDL_BUTTON_MIDDLE: return MyGUI::MouseButton::Middle;
    case SDL_BUTTON_RIGHT:  return MyGUI::MouseButton::Right;
    default:
        return MyGUI::MouseButton::None;
    }
}

//------------------------------------------------------------------------------

MyGUI::Char SDLKeycodeToMyGUI(SDL_Keycode code)
{
    MyGUI::Char c = 0;

    // ASCII codes
    if (code >= 0x00 && code <= 0x7F )
        c = code;

    //LOG << char(sym) << std::endl;
    return c;
}

//------------------------------------------------------------------------------

MyGUI::KeyCode SDLScancodeToMyGUI(SDL_Scancode code)
{
    using namespace MyGUI;

    switch (code) {

    case SDL_SCANCODE_UNKNOWN: return KeyCode::None;

    case SDL_SCANCODE_A: return KeyCode::A;
    case SDL_SCANCODE_B: return KeyCode::B;
    case SDL_SCANCODE_C: return KeyCode::C;
    case SDL_SCANCODE_D: return KeyCode::D;
    case SDL_SCANCODE_E: return KeyCode::E;
    case SDL_SCANCODE_F: return KeyCode::F;
    case SDL_SCANCODE_G: return KeyCode::G;
    case SDL_SCANCODE_H: return KeyCode::H;
    case SDL_SCANCODE_I: return KeyCode::I;
    case SDL_SCANCODE_J: return KeyCode::J;
    case SDL_SCANCODE_K: return KeyCode::K;
    case SDL_SCANCODE_L: return KeyCode::L;
    case SDL_SCANCODE_M: return KeyCode::M;
    case SDL_SCANCODE_N: return KeyCode::N;
    case SDL_SCANCODE_O: return KeyCode::O;
    case SDL_SCANCODE_P: return KeyCode::P;
    case SDL_SCANCODE_Q: return KeyCode::Q;
    case SDL_SCANCODE_R: return KeyCode::R;
    case SDL_SCANCODE_S: return KeyCode::S;
    case SDL_SCANCODE_T: return KeyCode::T;
    case SDL_SCANCODE_U: return KeyCode::U;
    case SDL_SCANCODE_V: return KeyCode::V;
    case SDL_SCANCODE_W: return KeyCode::W;
    case SDL_SCANCODE_X: return KeyCode::X;
    case SDL_SCANCODE_Y: return KeyCode::Y;
    case SDL_SCANCODE_Z: return KeyCode::Z;

    case SDL_SCANCODE_1: return KeyCode::One;
    case SDL_SCANCODE_2: return KeyCode::Two;
    case SDL_SCANCODE_3: return KeyCode::Three;
    case SDL_SCANCODE_4: return KeyCode::Four;
    case SDL_SCANCODE_5: return KeyCode::Five;
    case SDL_SCANCODE_6: return KeyCode::Six;
    case SDL_SCANCODE_7: return KeyCode::Seven;
    case SDL_SCANCODE_8: return KeyCode::Eight;
    case SDL_SCANCODE_9: return KeyCode::Nine;
    case SDL_SCANCODE_0: return KeyCode::Zero;

    case SDL_SCANCODE_RETURN: return KeyCode::Return;
    case SDL_SCANCODE_ESCAPE: return KeyCode::Escape;
    case SDL_SCANCODE_BACKSPACE: return KeyCode::Backspace;
    case SDL_SCANCODE_TAB: return KeyCode::Tab;
    case SDL_SCANCODE_SPACE: return KeyCode::Space;

    case SDL_SCANCODE_MINUS: return KeyCode::Minus;
    case SDL_SCANCODE_EQUALS: return KeyCode::Equals;
    case SDL_SCANCODE_LEFTBRACKET: return KeyCode::LeftBracket;
    case SDL_SCANCODE_RIGHTBRACKET: return KeyCode::RightBracket;
    case SDL_SCANCODE_BACKSLASH: return KeyCode::Backslash;

    //case SDL_SCANCODE_NONUSHASH: return KeyCode::

    case SDL_SCANCODE_SEMICOLON: return KeyCode::Semicolon;
    case SDL_SCANCODE_APOSTROPHE: return KeyCode::Apostrophe;
    case SDL_SCANCODE_GRAVE: return KeyCode::Grave;

    case SDL_SCANCODE_COMMA: return KeyCode::Comma;
    case SDL_SCANCODE_PERIOD: return KeyCode::Period;
    case SDL_SCANCODE_SLASH: return KeyCode::Slash;

    //case SDL_SCANCODE_CAPSLOCK: return KeyCode::

    case SDL_SCANCODE_F1: return KeyCode::F1;
    case SDL_SCANCODE_F2: return KeyCode::F2;
    case SDL_SCANCODE_F3: return KeyCode::F3;
    case SDL_SCANCODE_F4: return KeyCode::F4;
    case SDL_SCANCODE_F5: return KeyCode::F5;
    case SDL_SCANCODE_F6: return KeyCode::F6;
    case SDL_SCANCODE_F7: return KeyCode::F7;
    case SDL_SCANCODE_F8: return KeyCode::F8;
    case SDL_SCANCODE_F9: return KeyCode::F9;
    case SDL_SCANCODE_F10: return KeyCode::F10;
    case SDL_SCANCODE_F11: return KeyCode::F11;
    case SDL_SCANCODE_F12: return KeyCode::F12;

    case SDL_SCANCODE_PRINTSCREEN: return KeyCode::SysRq;
    case SDL_SCANCODE_SCROLLLOCK: return KeyCode::ScrollLock;
    case SDL_SCANCODE_PAUSE: return KeyCode::Pause;
    case SDL_SCANCODE_INSERT: return KeyCode::Insert;

    case SDL_SCANCODE_HOME: return KeyCode::Home;
    case SDL_SCANCODE_PAGEUP: return KeyCode::PageUp;
    case SDL_SCANCODE_DELETE: return KeyCode::Delete;
    case SDL_SCANCODE_END: return KeyCode::End;
    case SDL_SCANCODE_PAGEDOWN: return KeyCode::PageDown;
    case SDL_SCANCODE_RIGHT: return KeyCode::ArrowRight;
    case SDL_SCANCODE_LEFT: return KeyCode::ArrowLeft;
    case SDL_SCANCODE_DOWN: return KeyCode::ArrowDown;
    case SDL_SCANCODE_UP: return KeyCode::ArrowUp;

    case SDL_SCANCODE_NUMLOCKCLEAR: return KeyCode::NumLock;

    case SDL_SCANCODE_KP_DIVIDE: return KeyCode::Divide;
    case SDL_SCANCODE_KP_MULTIPLY: return KeyCode::Multiply;
    //case SDL_SCANCODE_KP_MINUS: return KeyCode::Minus;
    //case SDL_SCANCODE_KP_PLUS: return KeyCode::
    case SDL_SCANCODE_KP_ENTER: return KeyCode::NumpadEnter;
    case SDL_SCANCODE_KP_1: return KeyCode::Numpad1;
    case SDL_SCANCODE_KP_2: return KeyCode::Numpad2;
    case SDL_SCANCODE_KP_3: return KeyCode::Numpad3;
    case SDL_SCANCODE_KP_4: return KeyCode::Numpad4;
    case SDL_SCANCODE_KP_5: return KeyCode::Numpad5;
    case SDL_SCANCODE_KP_6: return KeyCode::Numpad6;
    case SDL_SCANCODE_KP_7: return KeyCode::Numpad7;
    case SDL_SCANCODE_KP_8: return KeyCode::Numpad8;
    case SDL_SCANCODE_KP_9: return KeyCode::Numpad9;
    case SDL_SCANCODE_KP_0: return KeyCode::Numpad0;
    //case SDL_SCANCODE_KP_PERIOD: return KeyCode::NumpadComma;

    //case SDL_SCANCODE_NONUSBACKSLASH: return KeyCode::

    //case SDL_SCANCODE_APPLICATION: return KeyCode::AppMenu;
    case SDL_SCANCODE_POWER: return KeyCode::Power;

    case SDL_SCANCODE_KP_EQUALS: return KeyCode::NumpadEquals;
    case SDL_SCANCODE_F13: return KeyCode::F13;
    case SDL_SCANCODE_F14: return KeyCode::F14;
    case SDL_SCANCODE_F15: return KeyCode::F15;
    //case SDL_SCANCODE_F16: return KeyCode::
    //case SDL_SCANCODE_F17: return KeyCode::
    //case SDL_SCANCODE_F18: return KeyCode::
    //case SDL_SCANCODE_F19: return KeyCode::
    //case SDL_SCANCODE_F20: return KeyCode::
    //case SDL_SCANCODE_F21: return KeyCode::
    //case SDL_SCANCODE_F22: return KeyCode::
    //case SDL_SCANCODE_F23: return KeyCode::
    //case SDL_SCANCODE_F24: return KeyCode::
    //case SDL_SCANCODE_EXECUTE: return KeyCode::
    //case SDL_SCANCODE_HELP: return KeyCode::
    case SDL_SCANCODE_MENU: return KeyCode::AppMenu;
    //case SDL_SCANCODE_SELECT: return KeyCode::
    case SDL_SCANCODE_STOP: return KeyCode::Stop;
    //case SDL_SCANCODE_AGAIN: return KeyCode::
    //case SDL_SCANCODE_UNDO: return KeyCode::
    //case SDL_SCANCODE_CUT: return KeyCode::
    //case SDL_SCANCODE_COPY: return KeyCode::
    //case SDL_SCANCODE_PASTE: return KeyCode::
    //case SDL_SCANCODE_FIND: return KeyCode::
    case SDL_SCANCODE_MUTE: return KeyCode::Mute;
    case SDL_SCANCODE_VOLUMEUP: return KeyCode::VolumeUp;
    case SDL_SCANCODE_VOLUMEDOWN: return KeyCode::VolumeDown;

    /*
    case SDL_SCANCODE_INTERNATIONAL2: return KeyCode::
    case SDL_SCANCODE_INTERNATIONAL3: return KeyCode::
    case SDL_SCANCODE_INTERNATIONAL4: return KeyCode::
    case SDL_SCANCODE_INTERNATIONAL5: return KeyCode::
    case SDL_SCANCODE_INTERNATIONAL6: return KeyCode::
    case SDL_SCANCODE_INTERNATIONAL7: return KeyCode::
    case SDL_SCANCODE_INTERNATIONAL8: return KeyCode::
    case SDL_SCANCODE_INTERNATIONAL9: return KeyCode::
    case SDL_SCANCODE_LANG1: return KeyCode::
    case SDL_SCANCODE_LANG2: return KeyCode::
    case SDL_SCANCODE_LANG3: return KeyCode::
    case SDL_SCANCODE_LANG4: return KeyCode::
    case SDL_SCANCODE_LANG5: return KeyCode::
    case SDL_SCANCODE_LANG6: return KeyCode::
    case SDL_SCANCODE_LANG7: return KeyCode::
    case SDL_SCANCODE_LANG8: return KeyCode::
    case SDL_SCANCODE_LANG9: return KeyCode::

    case SDL_SCANCODE_ALTERASE: return KeyCode::
    case SDL_SCANCODE_SYSREQ: return KeyCode::
    case SDL_SCANCODE_CANCEL: return KeyCode::
    case SDL_SCANCODE_CLEAR: return KeyCode::
    case SDL_SCANCODE_PRIOR: return KeyCode::
    case SDL_SCANCODE_RETURN2: return KeyCode::
    case SDL_SCANCODE_SEPARATOR: return KeyCode::
    case SDL_SCANCODE_OUT: return KeyCode::
    case SDL_SCANCODE_OPER: return KeyCode::
    case SDL_SCANCODE_CLEARAGAIN: return KeyCode::
    case SDL_SCANCODE_CRSEL: return KeyCode::
    case SDL_SCANCODE_EXSEL: return KeyCode::

    case SDL_SCANCODE_KP_00: return KeyCode::
    case SDL_SCANCODE_KP_000: return KeyCode::
    case SDL_SCANCODE_THOUSANDSSEPARATOR: return KeyCode::
    case SDL_SCANCODE_DECIMALSEPARATOR: return KeyCode::
    case SDL_SCANCODE_CURRENCYUNIT: return KeyCode::
    case SDL_SCANCODE_CURRENCYSUBUNIT: return KeyCode::
    case SDL_SCANCODE_KP_LEFTPAREN: return KeyCode::
    case SDL_SCANCODE_KP_RIGHTPAREN: return KeyCode::
    case SDL_SCANCODE_KP_LEFTBRACE: return KeyCode::
    case SDL_SCANCODE_KP_RIGHTBRACE: return KeyCode::
    case SDL_SCANCODE_KP_TAB: return KeyCode::
    case SDL_SCANCODE_KP_BACKSPACE: return KeyCode::
    case SDL_SCANCODE_KP_A: return KeyCode::
    case SDL_SCANCODE_KP_B: return KeyCode::
    case SDL_SCANCODE_KP_C: return KeyCode::
    case SDL_SCANCODE_KP_D: return KeyCode::
    case SDL_SCANCODE_KP_E: return KeyCode::
    case SDL_SCANCODE_KP_F: return KeyCode::
    case SDL_SCANCODE_KP_XOR: return KeyCode::
    case SDL_SCANCODE_KP_POWER: return KeyCode::
    case SDL_SCANCODE_KP_PERCENT: return KeyCode::
    case SDL_SCANCODE_KP_LESS: return KeyCode::
    case SDL_SCANCODE_KP_GREATER: return KeyCode::
    case SDL_SCANCODE_KP_AMPERSAND: return KeyCode::
    case SDL_SCANCODE_KP_DBLAMPERSAND: return KeyCode::
    case SDL_SCANCODE_KP_VERTICALBAR: return KeyCode::
    case SDL_SCANCODE_KP_DBLVERTICALBAR: return KeyCode::
    case SDL_SCANCODE_KP_COLON: return KeyCode::
    case SDL_SCANCODE_KP_HASH: return KeyCode::
    case SDL_SCANCODE_KP_SPACE: return KeyCode::
    case SDL_SCANCODE_KP_AT: return KeyCode::
    case SDL_SCANCODE_KP_EXCLAM: return KeyCode::
    case SDL_SCANCODE_KP_MEMSTORE: return KeyCode::
    case SDL_SCANCODE_KP_MEMRECALL: return KeyCode::
    case SDL_SCANCODE_KP_MEMCLEAR: return KeyCode::
    case SDL_SCANCODE_KP_MEMADD: return KeyCode::
    case SDL_SCANCODE_KP_MEMSUBTRACT: return KeyCode::
    case SDL_SCANCODE_KP_MEMMULTIPLY: return KeyCode::
    case SDL_SCANCODE_KP_MEMDIVIDE: return KeyCode::
    case SDL_SCANCODE_KP_PLUSMINUS: return KeyCode::
    case SDL_SCANCODE_KP_CLEAR: return KeyCode::
    case SDL_SCANCODE_KP_CLEARENTRY: return KeyCode::
    case SDL_SCANCODE_KP_BINARY: return KeyCode::
    case SDL_SCANCODE_KP_OCTAL: return KeyCode::
    case SDL_SCANCODE_KP_DECIMAL: return KeyCode::
    case SDL_SCANCODE_KP_HEXADECIMAL: return KeyCode::
    */
    case SDL_SCANCODE_LCTRL: return KeyCode::LeftControl;
    case SDL_SCANCODE_LSHIFT: return KeyCode::LeftShift;
    case SDL_SCANCODE_LALT: return KeyCode::LeftAlt;
    case SDL_SCANCODE_LGUI: return KeyCode::LeftWindows;
    case SDL_SCANCODE_RCTRL: return KeyCode::RightControl;
    case SDL_SCANCODE_RSHIFT: return KeyCode::RightShift;
    case SDL_SCANCODE_RALT: return KeyCode::RightAlt;
    case SDL_SCANCODE_RGUI: return KeyCode::RightWindows;

    //case SDL_SCANCODE_MODE: return KeyCode::

    case SDL_SCANCODE_AUDIONEXT: return KeyCode::NextTrack;
    case SDL_SCANCODE_AUDIOPREV: return KeyCode::PrevTrack;
    case SDL_SCANCODE_AUDIOSTOP: return KeyCode::MediaStop;
    case SDL_SCANCODE_AUDIOPLAY: return KeyCode::PlayPause;
    case SDL_SCANCODE_AUDIOMUTE: return KeyCode::Mute;
    case SDL_SCANCODE_MEDIASELECT: return KeyCode::MediaSelect;
    case SDL_SCANCODE_WWW: return KeyCode::WebHome; // Not sure
    case SDL_SCANCODE_MAIL: return KeyCode::Mail;
    case SDL_SCANCODE_CALCULATOR: return KeyCode::Calculator;
    case SDL_SCANCODE_COMPUTER: return KeyCode::MyComputer;
    case SDL_SCANCODE_AC_SEARCH: return KeyCode::WebSearch;
    case SDL_SCANCODE_AC_HOME: return KeyCode::WebHome;
    case SDL_SCANCODE_AC_BACK: return KeyCode::WebBack;
    case SDL_SCANCODE_AC_FORWARD: return KeyCode::WebForward;
    case SDL_SCANCODE_AC_STOP: return KeyCode::WebStop;
    case SDL_SCANCODE_AC_REFRESH: return KeyCode::WebRefresh;
    case SDL_SCANCODE_AC_BOOKMARKS: return KeyCode::WebFavorites;
    /*
    case SDL_SCANCODE_BRIGHTNESSDOWN: return KeyCode::
    case SDL_SCANCODE_BRIGHTNESSUP: return KeyCode::
    case SDL_SCANCODE_DISPLAYSWITCH: return KeyCode::

    case SDL_SCANCODE_KBDILLUMTOGGLE: return KeyCode::
    case SDL_SCANCODE_KBDILLUMDOWN: return KeyCode::
    case SDL_SCANCODE_KBDILLUMUP: return KeyCode::
    case SDL_SCANCODE_EJECT: return KeyCode::
    case SDL_SCANCODE_SLEEP: return KeyCode::

    case SDL_SCANCODE_APP1: return KeyCode::
    case SDL_SCANCODE_APP2: return KeyCode::
    */
    default: return KeyCode::None;
    }
}
