/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "RemoteGameView.h"

using namespace net;
using namespace event;

RemoteGameView::RemoteGameView(int socketId)
    : m_nef(socketId)
{
    m_socketId = socketId;
    m_viewId = -1;
    m_actorId = 0;

    elh.registerListener(ACTOR_MOVED,                 std::bind(&NetworkEventForwarder::forwardEvent, &m_nef, std::placeholders::_1));
    elh.registerListener(ACTOR_PHYSICS_STATE_CHANGED, std::bind(&NetworkEventForwarder::forwardEvent, &m_nef, std::placeholders::_1));
    elh.registerListener(ACTOR_CREATED,               std::bind(&NetworkEventForwarder::forwardEvent, &m_nef, std::placeholders::_1));
    elh.registerListener(ACTOR_DESTROYED,             std::bind(&NetworkEventForwarder::forwardEvent, &m_nef, std::placeholders::_1));
}
