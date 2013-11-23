#ifndef CLIENTSOCKETMGR_H
#define CLIENTSOCKETMGR_H

#include "BaseSocketMgr.h"

#include <string>

namespace net {

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

} // namespace network

#endif // CLIENTSOCKETMGR_H
