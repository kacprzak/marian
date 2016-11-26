/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#include "ClientSocketMgr.h"

#include "RemoteEventSocket.h"

using namespace net;

ClientSocketMgr::ClientSocketMgr(const std::string& hostName, unsigned int port)
    : m_hostName(hostName)
    , m_port(port)
{
}

//------------------------------------------------------------------------------

bool ClientSocketMgr::connect()
{
    RemoteEventSocket* socket = new RemoteEventSocket;

    if (!socket->connect(getHostByName(m_hostName), m_port)) {
        delete socket;
        return false;
    }

    addSocket(socket);
    return true;
}
