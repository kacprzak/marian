/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
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

    registerHandlers();
    LOG << "created Console\n";
}

//------------------------------------------------------------------------------

Console::~Console()
{
    MyGUI::Gui::getInstance().destroyWidget(m_consoleWindow);
    LOG << "destroyed Console\n";
}

//------------------------------------------------------------------------------

void Console::handleKey(MyGUI::KeyCode key)
{
    switch (key.toValue()) {
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

    if(visible)
        m_comboCommand->setEnabled(true);
    else
        m_comboCommand->setEnabled(false);
}

//------------------------------------------------------------------------------

bool Console::isVisible()
{
    return m_consoleWindow->getVisible();
}

//------------------------------------------------------------------------------

void Console::registerHandlers()
{
    m_buttonSubmit->eventMouseButtonClick += MyGUI::newDelegate(this, &Console::handle_SendButtonPressed);

    m_comboCommand->eventComboAccept += MyGUI::newDelegate(this, &Console::handle_ComboAccept);
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
        ScriptMgr::singleton().executeString(inMsg.asUTF8_c_str());
        outputText(m_echoColor + inMsg); // echo
    } catch (const ScriptError& ex) {
        outputText(m_errorColor + ex.what());
    }
}

//------------------------------------------------------------------------------

void Console::outputText(MyGUI::UString inMsg)
{
    if (m_listHistory->getCaption().empty())
        m_listHistory->addText(inMsg);
    else
        m_listHistory->addText("\n" + inMsg);

    //m_listHistory->setTextCursor(0);
    m_listHistory->setTextSelection(m_listHistory->getTextLength(), m_listHistory->getTextLength());
}

//------------------------------------------------------------------------------

void Console::clearText()
{
    m_listHistory->setCaption("");
}

//------------------------------------------------------------------------------

void Console::revertPreviousCommand()
{
    //m_consoleWindow->getChild("Vanilla/Console/Editbox")->setText(m_previousCmd);
}
