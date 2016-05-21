/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "BaseSocketMgr.h"

#include "config.h"
#include "Logger.h"

#if PLATFORM == PLATFORM_WINDOWS
  #include <winsock2.h>
#else
  #include <unistd.h> // close
  #include <netdb.h>  // hostent
#endif

#include <string>

using namespace net;

BaseSocketMgr::BaseSocketMgr()
    : m_nextSocketId(1)
    , m_inbound(0)
    , m_outbound(0)
    , m_maxOpenSockets(0)
    , m_subnet(0xffffffff)
    , m_subnetMask(0)
{
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    int err = WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
    if (err != NO_ERROR) {
        std::string msg("WSAStartup failed with error: ");
        msg += std::to_string(err);
        LOG_FATAL << msg << std::endl;
        throw std::runtime_error(msg);
    }
#endif
}

//------------------------------------------------------------------------------

BaseSocketMgr::~BaseSocketMgr()
{
    shutdown();

#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
#endif

    LOG << "Data sent: " << m_outbound << " bytes | Data recv: " << m_inbound << " bytes" << std::endl;
}

//------------------------------------------------------------------------------

void BaseSocketMgr::shutdown()
{
    for(NetSocket *sock : m_sockList) {
        delete sock;
    }
    m_sockList.clear();
}

//------------------------------------------------------------------------------

int BaseSocketMgr::addSocket(NetSocket *socket)
{
    socket->m_id = m_nextSocketId;
    m_sockMap[m_nextSocketId++] = socket;

    m_sockList.push_front(socket);

    if (m_sockList.size() > m_maxOpenSockets)
        ++m_maxOpenSockets;

    return socket->m_id;
}

//------------------------------------------------------------------------------

void BaseSocketMgr::removeSocket(NetSocket *socket)
{
    m_sockMap.erase(socket->m_id);
    m_sockList.remove(socket);

    delete socket;
}

//------------------------------------------------------------------------------

bool BaseSocketMgr::send(int sockId, std::shared_ptr<Packet> packet)
{
    NetSocket *sock = findSocket(sockId);

    if (!sock)
        return false;

    sock->send(packet);
    return true;
}

//------------------------------------------------------------------------------

void BaseSocketMgr::select(int pauseMicroSecs, bool handleInput)
{
    // How long to wait
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = pauseMicroSecs;

    fd_set inp_set, out_set, exp_set;

    FD_ZERO(&inp_set);
    FD_ZERO(&out_set);
    FD_ZERO(&exp_set);

    int maxdesc = 0;

    for (NetSocket *sock : m_sockList) {
        if ((sock->m_deleteFlag & 1) || sock->m_socket == -1)
            continue;

        if (handleInput)
            FD_SET(sock->m_socket, &inp_set);

        FD_SET(sock->m_socket, &exp_set);

        if (sock->hasOutput())
            FD_SET(sock->m_socket, &out_set);

        if (sock->m_socket > maxdesc)
            maxdesc = sock->m_socket;
    }

    // Hard work is done here
    int selectRet = ::select(maxdesc + 1, &inp_set, &out_set, &exp_set, &tv);
    if (selectRet == -1) {
        PLOG << "select";
        return;
    }

    // Handle sets
    if (selectRet) {
        for (NetSocket *sock : m_sockList) {
            if ((sock->m_deleteFlag & 1) || sock->m_socket == -1)
                continue;

            if (FD_ISSET(sock->m_socket, &exp_set))
                sock->handleException();

            if (!(sock->m_deleteFlag & 1) && FD_ISSET(sock->m_socket, &out_set))
                sock->handleOutput();

            if (!(sock->m_deleteFlag & 1) && FD_ISSET(sock->m_socket, &inp_set))
                sock->handleInput();
        }
    }

    time_t timeNow = ::time(NULL);

    auto it = std::begin(m_sockList);
    while (it != std::end(m_sockList)) {
        NetSocket *sock = *it;

        if (sock->m_timeOut && (sock->m_timeOut < timeNow))
            sock->timeOut();

        if (sock->m_deleteFlag & 1) {
            switch (sock->m_deleteFlag) {
            case 1:
                removeSocket(sock);
                it = std::begin(m_sockList);
                break;
            case 3:
                sock->m_deleteFlag = 2;
                if (sock->m_socket != -1) {
#if PLATFORM == PLATFORM_WINDOWS
                    closesocket(sock->m_socket);
#else
                    close(sock->m_socket);
#endif
                    sock->m_socket = -1;
                }
                break;
            default:
                break;
            }
        }
        ++it;
    }
}

//------------------------------------------------------------------------------

bool BaseSocketMgr::isInternal(unsigned int ip)
{
    if (!m_subnetMask)
        return false;

    if ((ip & m_subnetMask) == m_subnet)
        return false;

    return true;
}

//------------------------------------------------------------------------------

unsigned int BaseSocketMgr::getHostByName(const std::string& hostName)
{
    struct hostent *hostEnt = ::gethostbyname(hostName.c_str());

    if (!hostEnt) {
        PLOG << "gethostbyname";
        return 0;
    }

    struct sockaddr_in tmpSockAddr;

    memcpy(&tmpSockAddr.sin_addr, hostEnt->h_addr, hostEnt->h_length);
    return ntohl(tmpSockAddr.sin_addr.s_addr);
}

//------------------------------------------------------------------------------

const char *BaseSocketMgr::getHostByAddr(unsigned int ip)
{
    static char host[32];

    unsigned int netip = htonl(ip);

    struct hostent *hostEnt = ::gethostbyaddr((const char*)&netip, sizeof(netip), PF_INET);

    if (hostEnt == NULL) {
        PLOG << "gethostbyaddr";
        return nullptr;
    }

    strcpy(host, hostEnt->h_name);
    return host;
}

//------------------------------------------------------------------------------

NetSocket *BaseSocketMgr::findSocket(int sockId)
{
    SocketIdMap::iterator it = m_sockMap.find(sockId);
    if (it == std::end(m_sockMap))
        return nullptr;
    return it->second;
}
