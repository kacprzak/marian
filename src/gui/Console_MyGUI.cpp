/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Console_MyGUI.h"

#include "ScriptMgr.h"
#include "Logger.h"

#include <limits>

using namespace gui;

Console::Console()
    : m_consoleVisible(true)
{
    MyGUI::LayoutManager::getInstance().loadLayout("Console.layout");

    MyGUI::Gui& gui = MyGUI::Gui::getInstance();
    m_consoleWindow = gui.findWidget<MyGUI::Window>("_Main");
    m_listHistory   = gui.findWidget<MyGUI::EditBox>("list_History");
    m_comboCommand  = gui.findWidget<MyGUI::ComboBox>("combo_Command");
    m_buttonSubmit  = gui.findWidget<MyGUI::Button>("button_Submit");

    m_listHistory->setOverflowToTheLeft(true);

    m_errorColor = m_consoleWindow->getUserString("ErrorColor");
    m_echoColor = m_consoleWindow->getUserString("EchoColor");

    // Example commands
    m_comboCommand->addItem("addBox(x, y)");

    registerHandlers();

    ScriptListener listenerFun = std::bind(&Console::output, this,
                                           std::placeholders::_1, false);
    m_scriptListener.reset(new ScriptListener(listenerFun));
    ScriptMgr::singleton().addListener(ScriptMgr::OutputType::OUT, m_scriptListener);

    ScriptListener errListenerFun = std::bind(&Console::output,
                                              this, std::placeholders::_1, true);
    m_errScriptListener.reset(new ScriptListener(errListenerFun));
    ScriptMgr::singleton().addListener(ScriptMgr::OutputType::ERR, m_errScriptListener);

    LOG << "created Console\n";
}

//------------------------------------------------------------------------------

Console::~Console()
{
    ScriptMgr::singleton().removeListener(ScriptMgr::OutputType::OUT, m_scriptListener);
    ScriptMgr::singleton().removeListener(ScriptMgr::OutputType::ERR, m_errScriptListener);

    MyGUI::Gui::getInstance().destroyWidget(m_consoleWindow);
    LOG << "destroyed Console\n";
}

//------------------------------------------------------------------------------

void Console::handleKey(MyGUI::KeyCode key)
{
    //switch (key.toValue()) {
    switch (key.getValue()) {
    case MyGUI::KeyCode::ArrowUp :
        revertPreviousCommand();
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void Console::setVisible(bool visible)
{
    m_consoleWindow->setVisible(visible);
    m_consoleVisible = visible;

    if(visible) {
        m_comboCommand->setEnabled(true);
        MyGUI::InputManager::getInstance().setKeyFocusWidget(m_comboCommand);
    } else {
        m_comboCommand->setEnabled(false);
    }
}

//------------------------------------------------------------------------------

bool Console::isVisible()
{
    return m_consoleWindow->getVisible();
}

//------------------------------------------------------------------------------

void Console::registerHandlers()
{
    m_buttonSubmit->eventMouseButtonClick += MyGUI::newDelegate(this,
                                                 &Console::handle_SendButtonPressed);

    m_comboCommand->eventComboAccept += MyGUI::newDelegate(this,
                                            &Console::handle_ComboAccept);
}

//------------------------------------------------------------------------------

void Console::handle_SendButtonPressed(MyGUI::Widget * _sender)
{
    handle_ComboAccept(m_comboCommand, MyGUI::ITEM_NONE);
}

//------------------------------------------------------------------------------

void Console::handle_ComboAccept(MyGUI::ComboBox *_sender, size_t _index)
{
    const MyGUI::UString& command = _sender->getOnlyText();
    if (command == "") return;

    parseText(command);

    _sender->setCaption("");
}

//------------------------------------------------------------------------------

void Console::parseText(MyGUI::UString inMsg)
{
    if (inMsg.empty())
        return;

    // Remember command
    m_previousCmd = inMsg;

    if (inMsg.compare("clear") == 0) {
        clearText();
        return;
    }

    try {
        outputText(m_echoColor + "> " + inMsg); // echo
        ScriptMgr::singleton().executeString(inMsg.asUTF8_c_str());
    } catch (const ScriptError& ex) {
        // Just output by listener
        //outputText(m_errorColor + ex.what());
    }
}

//------------------------------------------------------------------------------

void Console::outputText(MyGUI::UString inMsg, bool err)
{
    MyGUI::UString color = m_echoColor;
    if (err)
        color = m_errorColor;

    if (m_listHistory->getCaption().empty())
        m_listHistory->addText(inMsg);
    else
        m_listHistory->addText("\n" + color + inMsg);

    //m_listHistory->setTextCursor(0);
    m_listHistory->setTextSelection(m_listHistory->getTextLength(),
                                    m_listHistory->getTextLength());
}

//------------------------------------------------------------------------------

void Console::output(const std::string& inMsg, bool err)
{
    outputText(MyGUI::UString(inMsg), err);
}

//------------------------------------------------------------------------------

void Console::clearText()
{
    m_listHistory->setCaption("");
}

//------------------------------------------------------------------------------

void Console::revertPreviousCommand()
{
    m_comboCommand->setCaption(m_previousCmd);
}
