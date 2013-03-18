/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "GuiMgr.h"

#include <CEGUI.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
#include <iostream>

GuiMgr::GuiMgr()
{
    CEGUI::OpenGLRenderer::bootstrapSystem();

    std::string folder = "media/cegui/";
    // Initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
        (CEGUI::System::getSingleton().getResourceProvider());

    rp->setResourceGroupDirectory("schemes",     folder + "schemes/");
    rp->setResourceGroupDirectory("imagesets",   folder + "imagesets/");
    rp->setResourceGroupDirectory("fonts",       folder + "fonts/");
    rp->setResourceGroupDirectory("layouts",     folder + "layouts/");
    rp->setResourceGroupDirectory("looknfeels",  folder + "looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", folder + "lua_scripts/");

    // Set the default resource groups to be used
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

    // Load the scheme
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme");

    CEGUI::Window *consoleWindow
        = CEGUI::WindowManager::getSingleton().loadWindowLayout("VanillaConsole.layout");

    // Set the defaults
    CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
    CEGUI::System::getSingleton().setDefaultMouseCursor("Vanilla-Images", "MouseArrow");

    CEGUI::Window* myRoot =
        CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "_MasterRoot");

    CEGUI::System::getSingleton().setGUISheet(myRoot);

    CEGUI::System::getSingleton().getGUISheet()->addChildWindow(consoleWindow);

    std::cout << "GuiMgr created\n";
}

GuiMgr::~GuiMgr()
{
    std::cout << "GuiMgr destroyed\n";
}

//------------------------------------------------------------------------------

bool GuiMgr::processInput(const SDL_Event& e)
{
    switch (e.type) {
        /* mouse motion handler */
    case SDL_MOUSEMOTION:
        /* we inject the mouse position directly. */
        CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(e.motion.x),
                                                          static_cast<float>(e.motion.y));
        break;
 
        /* mouse down handler */
    case SDL_MOUSEBUTTONDOWN:
        /* let a special function handle the mouse button down event */
        handle_mouse_down(e.button.button);
        break;
 
        /* mouse up handler */
    case SDL_MOUSEBUTTONUP:
        /* let a special function handle the mouse button up event */
        handle_mouse_up(e.button.button);
        break;
 
        /* key down */
    case SDL_KEYDOWN:
        /* to tell CEGUI that a key was pressed, we inject the scancode. */
        CEGUI::System::getSingleton().injectKeyDown(e.key.keysym.scancode);
        /* as for the character it's a litte more complicated.
         * we'll use for translated unicode value.
         * this is described in more detail below.
         */
        if ((e.key.keysym.unicode & 0xFF80) == 0) {
            CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode & 0x7F);
        }
        break;
 
        /* key up */
    case SDL_KEYUP:
        /* like before we inject the scancode directly. */
        CEGUI::System::getSingleton().injectKeyUp(e.key.keysym.scancode);
        break;
 
    case SDL_VIDEORESIZE:
        CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Size(e.resize.w,
                                                                           e.resize.h));
        break;
    }

    return true; // Keep running
}

//------------------------------------------------------------------------------

void GuiMgr::update(float elapsedTime)
{
    CEGUI::System::getSingleton().injectTimePulse(elapsedTime);
}

//------------------------------------------------------------------------------

void GuiMgr::draw()
{
    CEGUI::System::getSingleton().renderGUI();
}

//------------------------------------------------------------------------------

void GuiMgr::handle_mouse_down(Uint8 button)
{
    switch (button) {
    case SDL_BUTTON_LEFT:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
        break;
    case SDL_BUTTON_MIDDLE:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
        break;
    case SDL_BUTTON_RIGHT:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
        break;
 
    case SDL_BUTTON_WHEELDOWN:
        CEGUI::System::getSingleton().injectMouseWheelChange( -1 );
        break;
    case SDL_BUTTON_WHEELUP:
        CEGUI::System::getSingleton().injectMouseWheelChange( +1 );
        break;
    }
}

//------------------------------------------------------------------------------
 
void GuiMgr::handle_mouse_up(Uint8 button)
{
    switch (button) {
    case SDL_BUTTON_LEFT:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
        break;
    case SDL_BUTTON_MIDDLE:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
        break;
    case SDL_BUTTON_RIGHT:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
        break;
    }
}
