#ifndef BASESOCKETMGR_H
#define BASESOCKETMGR_H

#include "Singleton.h"
#include "NetSocket.h"

#include <map>

#define GAME_PORT 3456

class BaseSocketMgr : public Singleton<BaseSocketMgr>
{
public:
    BaseSocketMgr();
    virtual ~BaseSocketMgr();

    bool init();
    void shutdown();

    int addSocket(NetSocket *socket);
    void removeSocket(NetSocket *socket);

    bool send(int sockId, std::shared_ptr<Packet> packet);
    void select(int pauseMicroSecs, bool handleInput = true);

    void setSubnet(unsigned int subnet, unsigned int subnetMask) {
        m_subnet = subnet;
        m_subnetMask = subnetMask;
    }

    bool isInternal(unsigned int ip);

    unsigned int getHostByName(const std::string& hostName);
    const char * getHostByAddr(unsigned int ip);

    void addToOutbound(int rc) { m_outbound += rc; }
    void addToInbound(int rc)  { m_inbound += rc; }

protected:
    typedef std::list<NetSocket *> SocketList;
    typedef std::map<int, NetSocket *> SocketIdMap;

    SocketList  m_sockList;        ///< List of sockets
    SocketIdMap m_sockMap;         ///< Map from id to socket handles

    int m_nextSocketId;

    unsigned int m_inbound;        // statistics gathering - inbound data
    unsigned int m_outbound;       // statistics gathering - outbound data
    unsigned int m_maxOpenSockets; // statistics gathering - max open sockets

    unsigned int m_subnet;         ///< the subnet of the internal network
    unsigned int m_subnetMask;     ///< the subnet mask of the internal network

    NetSocket *findSocket(int sockId);
};

#endif // BASESOCKETMGR_H