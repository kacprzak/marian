#include "GameServerListenNetSocket.h"

#include "RemoteEventSocket.h"
#include "BaseSocketManager.h"
#include "events/Event.h"
#include "events/EventMgr.h"
#include "Logger.h"

GameServerListenNetSocket::GameServerListenNetSocket(int portNum)
{
    init(portNum);

    LOG << "Listening on port: " << portNum << std::endl;
}

//------------------------------------------------------------------------------

void GameServerListenNetSocket::handleInput()
{
    unsigned int ipaddr;
    int new_sock = acceptConnection(&ipaddr);

    struct linger ling;
    ling.l_onoff = 0;
    ling.l_linger = 0;
    if (setsockopt(new_sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) == -1) {
        PLOG << "setsockopt";
    }

    if (new_sock != -1) {
        RemoteEventSocket *sock = new RemoteEventSocket(new_sock, ipaddr);
        int sockId = BaseSocketManager::singleton().addSocket(sock);
        int ipAddr = sock->ipAdress();
        EventPtr e(new RemoteClientEvent(sockId, ipAddr));
        EventMgr::singleton().queueEvent(e);
    }
}
