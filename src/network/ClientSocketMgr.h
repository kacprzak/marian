#ifndef CLIENTSOCKETMGR_H
#define CLIENTSOCKETMGR_H

#include "BaseSocketMgr.h"

#include <string>

class ClientSocketMgr : public BaseSocketMgr
{
    typedef BaseSocketMgr super;

public:
    ClientSocketMgr(const std::string& hostName, unsigned int port);

    bool connect();

private:
    std::string m_hostName;
    unsigned int m_port;
};

#endif // CLIENTSOCKETMGR_H
