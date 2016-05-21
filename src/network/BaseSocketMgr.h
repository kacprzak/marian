/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef BASESOCKETMGR_H
#define BASESOCKETMGR_H

#include "Singleton.h"
#include "NetSocket.h"

#include <map>

#define GAME_PORT 3456

namespace net {

    class BaseSocketMgr : public Singleton<BaseSocketMgr>
    {
        public:
        BaseSocketMgr();
        virtual ~BaseSocketMgr();

        int addSocket(NetSocket *socket);
        void removeSocket(NetSocket *socket);

        bool send(int sockId, std::shared_ptr<Packet> packet);
        void select(int pauseMicroSecs, bool handleInput = true);

        void setSubnet(unsigned int subnet, unsigned int subnetMask)
        {
            m_subnet = subnet;
            m_subnetMask = subnetMask;
        }

        bool isInternal(unsigned int ip);

        unsigned int getHostByName(const std::string& hostName);
        const char * getHostByAddr(unsigned int ip);

        void addToOutbound(int rc) { m_outbound += rc; }
        void addToInbound(int rc)  { m_inbound += rc; }

        protected:
        void shutdown();

        using SocketList = std::list<NetSocket *>;
        using SocketIdMap = std::map<int, NetSocket *>;

        SocketList  m_sockList;        //!< List of sockets
        SocketIdMap m_sockMap;         //!< Map from id to socket handles

        int m_nextSocketId;

        unsigned int m_inbound;        // statistics gathering - inbound data
        unsigned int m_outbound;       // statistics gathering - outbound data
        unsigned int m_maxOpenSockets; // statistics gathering - max open sockets

        unsigned int m_subnet;         //!< the subnet of the internal network
        unsigned int m_subnetMask;     //!< the subnet mask of the internal network

        NetSocket *findSocket(int sockId);
    };

} // namespace network

#endif // BASESOCKETMGR_H
