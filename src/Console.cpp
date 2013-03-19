/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Console.h"
#include <iostream>

Console::Console()
{
    m_consoleWindow =
        CEGUI::WindowManager::getSingleton().loadWindowLayout("VanillaConsole.layout");
    m_consoleVisible = true;

    registerHandlers();
    std::clog << "Console created\n";
}

//------------------------------------------------------------------------------

Console::~Console()
{
    std::clog << "Console destroyed\n";
}

//------------------------------------------------------------------------------

void Console::setVisible(bool visible)
{
    m_consoleWindow->setVisible(visible);
    m_consoleVisible = visible;
 
    CEGUI::Window *editBox = m_consoleWindow->getChild("Vanilla/Console/Editbox");
    if(visible)
        editBox->activate();
    else
        editBox->deactivate();
}

//------------------------------------------------------------------------------

bool Console::isVisible()
{
    return m_consoleWindow->isVisible();
}

//------------------------------------------------------------------------------

void Console::registerHandlers()
{
    m_consoleWindow->getChild("Vanilla/Console/Submit")->
        subscribeEvent(CEGUI::PushButton::EventClicked,
                       CEGUI::Event::Subscriber(&Console::handle_SendButtonPressed, this));

    m_consoleWindow->getChild("Vanilla/Console/Editbox")->
        subscribeEvent(CEGUI::Editbox::EventTextAccepted,
                       CEGUI::Event::Subscriber(&Console::handle_TextSubmitted, this));
}

//------------------------------------------------------------------------------

bool Console::handle_TextSubmitted(const CEGUI::EventArgs& e)
{
    const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
 
    CEGUI::String msg = m_consoleWindow->getChild("Vanilla/Console/Editbox")->getText();
 
    parseText(msg);
 
    m_consoleWindow->getChild("Vanilla/Console/Editbox")->setText("");
 
    return true;
}

//------------------------------------------------------------------------------

bool Console::handle_SendButtonPressed(const CEGUI::EventArgs& /*e*/)
{
    CEGUI::String msg = m_consoleWindow->getChild("Vanilla/Console/Editbox")->getText();
    parseText(msg);
    m_consoleWindow->getChild("Vanilla/Console/Editbox")->setText("");
 
    return true;
}

//------------------------------------------------------------------------------

void Console::parseText(CEGUI::String inMsg)
{
    outputText(inMsg);
}

//------------------------------------------------------------------------------

void Console::outputText(CEGUI::String inMsg, CEGUI::colour /*colour*/)
{
    CEGUI::MultiLineEditbox *outputWindow =
        static_cast<CEGUI::MultiLineEditbox *>(m_consoleWindow->getChild("Vanilla/Console/History"));
        
    outputWindow->setText(inMsg);
}
