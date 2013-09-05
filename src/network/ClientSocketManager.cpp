#include "ClientSocketManager.h"

#include "RemoteEventSocket.h"

ClientSocketManager::ClientSocketManager(const std::string &hostName, unsigned int port)
    : m_hostName(hostName)
    , m_port(port)
{
}

//------------------------------------------------------------------------------

bool ClientSocketManager::connect()
{
    if (!super::init())
        return false;

    RemoteEventSocket *socket = new RemoteEventSocket;

    if (!socket->connect(getHostByName(m_hostName), m_port)) {
        delete socket;
        return -1;
    }

    return addSocket(socket);
}
