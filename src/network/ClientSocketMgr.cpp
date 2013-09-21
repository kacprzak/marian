#include "ClientSocketMgr.h"

#include "RemoteEventSocket.h"

ClientSocketMgr::ClientSocketMgr(const std::string &hostName, unsigned int port)
    : m_hostName(hostName)
    , m_port(port)
{
}

//------------------------------------------------------------------------------

bool ClientSocketMgr::connect()
{
    if (!super::init())
        return false;

    RemoteEventSocket *socket = new RemoteEventSocket;

    if (!socket->connect(getHostByName(m_hostName), m_port)) {
        delete socket;
        return false;
    }

    addSocket(socket);
    return true;
}
