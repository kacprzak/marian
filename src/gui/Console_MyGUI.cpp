/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Console_MyGUI.h"

#include "ScriptMgr.h"
#include "Logger.h"

#include <limits>

using namespace gui;

Console::Console()
{
    MyGUI::LayoutManager::getInstance().loadLayout("Console.layout");
    m_consoleWindow = MyGUI::Gui::getInstance().findWidget<MyGUI::Window>("_Main");

    m_consoleVisible = true;

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
 
    /*CEGUI::Window *editBox = m_consoleWindow->getChild("Vanilla/Console/Editbox");
    if(visible)
        editBox->activate();
    else
        editBox->deactivate();
*/
}

//------------------------------------------------------------------------------

bool Console::isVisible()
{
    return m_consoleWindow->getVisible();
}

//------------------------------------------------------------------------------

void Console::registerHandlers()
{
    MyGUI::Button *submitBtn = MyGUI::Gui::getInstance().findWidget<MyGUI::Button>("button_Submit");
    submitBtn->eventMouseButtonClick += MyGUI::newDelegate(this, &Console::handle_SendButtonPressed);

    /*
    m_consoleWindow->getChild("Vanilla/Console/Submit")->
        subscribeEvent(CEGUI::PushButton::EventClicked,
                       CEGUI::Event::Subscriber(&Console::handle_SendButtonPressed, this));

    m_consoleWindow->getChild("Vanilla/Console/Editbox")->
        subscribeEvent(CEGUI::Editbox::EventTextAccepted,
                       CEGUI::Event::Subscriber(&Console::handle_TextSubmitted, this));
*/
}

//------------------------------------------------------------------------------

bool Console::handle_TextSubmitted(MyGUI::Widget * _sender)
{
 /*
    //const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
 
    CEGUI::String msg = m_consoleWindow->getChild("Vanilla/Console/Editbox")->getText();
 
    parseText(msg);
 
    m_consoleWindow->getChild("Vanilla/Console/Editbox")->setText("");
 */
    return true;
}

//------------------------------------------------------------------------------

void Console::handle_SendButtonPressed(MyGUI::Widget * _sender)
{
    /*
    CEGUI::String msg = m_consoleWindow->getChild("Vanilla/Console/Editbox")->getText();
    parseText(msg);
    m_consoleWindow->getChild("Vanilla/Console/Editbox")->setText("");
 */
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
        outputText(inMsg); // echo
    } catch (const ScriptError& ex) {
        outputText(ex.what());
    }
}

//------------------------------------------------------------------------------

void Console::outputText(MyGUI::UString inMsg /*, CEGUI::colour /*colour*/)
{
    /*
    CEGUI::MultiLineEditbox *outWin =
        static_cast<CEGUI::MultiLineEditbox *>
        (m_consoleWindow->getChild("Vanilla/Console/History"));

    CEGUI::String text = outWin->getText();
    text.append(inMsg);
    outWin->setText(text);

    // Push scrollbar to bottom
    CEGUI::Scrollbar *sb = outWin->getVertScrollbar();
    sb->setScrollPosition(std::numeric_limits<float>::max());
*/
}

//------------------------------------------------------------------------------

void Console::clearText()
{
    /*
    CEGUI::Window *outputWindow = m_consoleWindow->getChild("Vanilla/Console/History");
    outputWindow->setText(CEGUI::String());
*/
}

//------------------------------------------------------------------------------

void Console::revertPreviousCommand()
{
    //m_consoleWindow->getChild("Vanilla/Console/Editbox")->setText(m_previousCmd);
}
