/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "RemoteEventSocket.h"

#include "events/EventMgr.h"
#include "events/EventType.h"
#include "events/Event.h"
#include "Logger.h"

#include <sstream>

using namespace net;

RemoteEventSocket::RemoteEventSocket()
{}

//------------------------------------------------------------------------------

RemoteEventSocket::RemoteEventSocket(int socket, unsigned int ip)
    : NetSocket(socket, ip)
{}

//------------------------------------------------------------------------------

void RemoteEventSocket::handleInput()
{
    super::handleInput();

    using namespace event;

    while (!m_inList.empty()) {
        std::shared_ptr<Packet> packet = *(m_inList.begin());
        m_inList.pop_front();

        const char *buf = packet->getData();
        int size = packet->getSize();

        static char str[MAX_PACKET_SIZE + 1];

        memcpy(str, buf, size);
        str[size] = '\0';
        std::istringstream in(str + sizeof(uint32));

        unsigned short eventTypeVal;
        in >> eventTypeVal;
        EventType eventType = static_cast<EventType>(eventTypeVal);

        EventMgr& evtMgr = EventMgr::singleton();
        std::unique_ptr<Event> eventPtr;

        switch (eventType) {
        case ACTOR_MOVED:
        {
            eventPtr.reset(new MoveEvent(in));
            break;
        }
        case ACTOR_PHYSICS_STATE_CHANGED:
        {
            eventPtr.reset(new PhysicsStateChangeEvent(in));
            break;
        }
        case INPUT_COMMAND:
            eventPtr.reset(new ActorInputEvent(in));
            break;
        }

        if (eventPtr)
            evtMgr.queueEvent(std::move(eventPtr));
    }
}
