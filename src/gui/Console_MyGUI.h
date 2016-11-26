#ifndef CONSOLE_H
#define CONSOLE_H

#include "ScriptMgr.h"
#include "Singleton.h"
#include <MyGUI.h>

#include <functional>

namespace gui {

class Console : public ScriptListener
{
  public:
    Console();
    ~Console();

    MyGUI::Window* getWindow() { return m_consoleWindow; }

    void handleKey(MyGUI::KeyCode key);

    // Hide or show the console
    void setVisible(bool visible);
    // return true if console is visible, false if is hidden
    bool isVisible();
    void toggleVisible() { setVisible(!m_consoleVisible); }

    void revertPreviousCommand();

    // Inherited via ScriptListener
    virtual void onScriptOutput(const std::string& out) override;
    virtual void onScriptError(const std::string& out) override;

  private:
    // Register our handler functions
    void registerHandlers();
    // Handle when we press the Send button
    void handle_SendButtonPressed(MyGUI::Widget* _sender);
    // Handle when we press Enter after typing
    void handle_ComboAccept(MyGUI::ComboBox* _sender, size_t _index);
    // Parse the text the user submitted
    void parseText(MyGUI::UString inMsg);
    // Post the message to the ChatHistory listbox
    void outputText(MyGUI::UString inMsg, bool err = false);
    void output(const std::string& inMsg, bool err = false);

    void clearText();

  private:
    bool m_consoleVisible;

    MyGUI::Window* m_consoleWindow;
    MyGUI::EditBox* m_listHistory;
    MyGUI::ComboBox* m_comboCommand;
    MyGUI::Button* m_buttonSubmit;

    MyGUI::UString m_errorColor;
    MyGUI::UString m_echoColor;
    MyGUI::UString m_previousCmd;
};

} // namespace gui

#endif // CONSOLE_H
