/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#include "NetworkEventForwarder.h"

#include "Packet.h"

#include <sstream>

using namespace net;

NetworkEventForwarder::NetworkEventForwarder(int socketId)
{
    m_socketId = socketId;
}

//------------------------------------------------------------------------------

void NetworkEventForwarder::forwardEvent(event::Event& e)
{
    std::ostringstream out;

    out << static_cast<unsigned short>(e.eventType()) << " ";
    // out << e.eventName() << " ";
    out << e;
    // out << "\r\n";

    const std::string& data = out.str();

    std::shared_ptr<Packet> packet(new Packet(data.c_str(), data.length()));

    BaseSocketMgr::singleton().send(m_socketId, packet);
}
