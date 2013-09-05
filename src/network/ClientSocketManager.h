#ifndef CLIENTSOCKETMANAGER_H
#define CLIENTSOCKETMANAGER_H

#include "BaseSocketManager.h"

class ClientSocketManager : public BaseSocketManager
{
    typedef BaseSocketManager super;

public:
    ClientSocketManager(const std::string& hostName, unsigned int port);

    bool connect();

private:
    std::string m_hostName;
    unsigned int m_port;
};

#endif // CLIENTSOCKETMANAGER_H
