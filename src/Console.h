/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef CONSOLE_H
#define CONSOLE_H

#include "Singleton.h"
#include <CEGUI.h>

class Console : public Singleton<Console>
{
 public:
    Console();
    ~Console();

    CEGUI::Window *getWindow() { return m_consoleWindow; }

    void handleKey(CEGUI::uint key);

    // Hide or show the console
    void setVisible(bool visible);
    // return true if console is visible, false if is hidden
    bool isVisible();
    void toggleVisible()
    {
        setVisible(!m_consoleVisible);
    }

    void revertPreviousCommand();

 private:
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

    void clearText();

    CEGUI::Window *m_consoleWindow;
    bool m_consoleVisible;
    CEGUI::String m_previousCmd;
};

#endif // CONSOLE_H
