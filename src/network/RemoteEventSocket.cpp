#include "RemoteEventSocket.h"

#include "EventMgr.h"
#include "EventType.h"
#include "Event.h"
#include "Logger.h"

#include <sstream>

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

    while (!m_inList.empty()) {
        std::shared_ptr<Packet> packet = *(m_inList.begin());
        m_inList.pop_front();

        const char *buf = packet->getData();
        int size = packet->getSize();

        // TODO: Try to do it with no new and delete
        char *str = new char[size + 1];
        memcpy(str, buf, size);
        str[size] = '\0';
        std::istringstream in(str + sizeof(uint32));
        delete [] str;

        unsigned short eventTypeVal;
        in >> eventTypeVal;
        EventType eventType = static_cast<EventType>(eventTypeVal);

        switch (eventType) {
        case ACTOR_MOVED:
        {
            EventPtr event(new MoveEvent(in));
            EventMgr::singleton().queueEvent(event);
            break;
        }
        case ACTOR_PHYSICS_STATE_CHANGED:
        {
            EventPtr event(new PhysicsStateChangeEvent(in));
            EventMgr::singleton().queueEvent(event);
            break;
        }
        case INPUT_COMMAND:
            EventPtr event(new ActorInputEvent(in));
            EventMgr::singleton().queueEvent(event);
            break;
        }

    }
}
