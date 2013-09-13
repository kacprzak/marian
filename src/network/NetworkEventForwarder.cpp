#include "NetworkEventForwarder.h"

#include "Packet.h"

#include <sstream>

NetworkEventForwarder::NetworkEventForwarder(int socketId)
{
    m_socketId = socketId;
}

//------------------------------------------------------------------------------

void NetworkEventForwarder::forwardEvent(EventPtr e)
{
    std::ostringstream out;

    out << static_cast<unsigned short>(e->eventType()) << " ";
    //out << e->eventName() << " ";
    e->serialize(out);
    //out << "\r\n";

    const std::string& data = out.str();

    std::shared_ptr<Packet> packet(new Packet(data.c_str(), data.length()));

    BaseSocketMgr::singleton().send(m_socketId, packet);
}
