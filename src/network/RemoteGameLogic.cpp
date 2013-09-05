#include "RemoteGameLogic.h"

#include "EventMgr.h"

RemoteGameLogic::RemoteGameLogic(int socketId)
    : m_nef(socketId)
{
    m_socketId = socketId;
    elh.registerListener(INPUT_COMMAND, std::bind(&NetworkEventForwarder::forwardEvent, &m_nef, std::placeholders::_1));
}

//------------------------------------------------------------------------------

void RemoteGameLogic::update(float /*elapsedTime*/)
{
    EventMgr::singleton().update();
}
