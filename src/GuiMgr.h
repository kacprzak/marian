/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GUIMGR_H
#define GUIMGR_H

#include <SDL.h>
#include "Singleton.h"
#include <CEGUI.h>

class GuiMgr : public Singleton<GuiMgr>
{
 public:
    GuiMgr();
    ~GuiMgr();

    bool processInput(const SDL_Event& event);
    void update(float elapsedTime);
    void draw();

 private:
    void handle_mouse_up(Uint8 button);
    void handle_mouse_down(Uint8 button);

    // Hide or show the console
    void setConsoleVisible(bool visible);
    // return true if console is visible, false if is hidden
    bool isConsoleVisible();
    // Register our handler functions
    void registerHandlers();
    // Handle when we press Enter after typing
    bool handle_TextSubmitted(const CEGUI::EventArgs& e);
    // Handle when we press the Send button
    bool handle_SendButtonPressed(const CEGUI::EventArgs& e);
    // Parse the text the user submitted.
    void parseText(CEGUI::String inMsg);
    // Post the message to the ChatHistory listbox.
    // with a white color default
    void outputText(CEGUI::String inMsg,
                    CEGUI::colour colour = CEGUI::colour(0xFFFFFFFF));


    CEGUI::Window *m_consoleWindow;
    bool m_consoleVisible;
};

#endif // GUIMGR_H
