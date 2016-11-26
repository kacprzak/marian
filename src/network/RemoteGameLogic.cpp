/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#include "RemoteGameLogic.h"

#include "events/EventMgr.h"

using namespace net;

RemoteGameLogic::RemoteGameLogic(int socketId) : m_nef(socketId)
{
    m_socketId = socketId;
    elh.registerListener(event::INPUT_COMMAND,
                         std::bind(&NetworkEventForwarder::forwardEvent, &m_nef,
                                   std::placeholders::_1));
}

//------------------------------------------------------------------------------

void RemoteGameLogic::update(float /*elapsedTime*/)
{
    event::EventMgr::singleton().update();
}
