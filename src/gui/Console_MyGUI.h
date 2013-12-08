/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef CONSOLE_H
#define CONSOLE_H

#include "Singleton.h"
#include <MyGUI.h>

namespace gui {

class Console
{
 public:
    Console();
    ~Console();

    MyGUI::Window *getWindow() { return m_consoleWindow; }

    void handleKey(MyGUI::KeyCode key);

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
    bool handle_TextSubmitted(MyGUI::Widget * _sender);
    // Handle when we press the Send button
    void handle_SendButtonPressed(MyGUI::Widget* _sender);
    // Parse the text the user submitted.
    void parseText(MyGUI::UString inMsg);
    // Post the message to the ChatHistory listbox.
    // with a white color default
    void outputText(MyGUI::UString inMsg /*, //CEGUI::colour colour = CEGUI::colour(0xFFFFFFFF) */);

    void clearText();

    MyGUI::Window *m_consoleWindow;
    bool m_consoleVisible;
    MyGUI::UString m_previousCmd;
};

} // namespace gui

#endif // CONSOLE_H
