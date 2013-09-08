/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "NetSocket.h"

#include "Logger.h"
#include "BaseSocketManager.h"

#include <unistd.h> // close
#include <sys/ioctl.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <netinet/tcp.h> // TCP_NODELAY
//#include <netinet/in.h>
//#include <signal.h>

NetSocket::NetSocket()
{
    m_socket = -1;
    m_deleteFlag = 0;
    m_sendOffset = 0;
    m_timeOut = 0;
    m_recvOffset = m_recvBegin = 0;
    m_internal = 0;
    //m_bBinaryProtocol = 1;

    LOG << "new NetSocket sock_fd=" << m_socket << " (" << this << ")" << std::endl;
}

NetSocket::NetSocket(int new_sock, unsigned int hostIp)
{
    m_socket = new_sock;
    m_deleteFlag = 0;
    m_sendOffset = 0;
    m_timeOut = 0;
    m_recvOffset = m_recvBegin = 0;
    m_internal = 0;

    m_timeCreated = std::time(NULL);
    m_ipaddr = hostIp;

    m_internal = BaseSocketManager::singleton().isInternal(hostIp);

    struct linger ling;
    ling.l_onoff = 0;
    ling.l_linger = 0;
    if (setsockopt(new_sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) == -1) {
        PLOG << "setsockopt";
    }

    LOG << "new NetSocket sock_fd=" << m_socket << " (" << this << ")" << std::endl;
}

//------------------------------------------------------------------------------

NetSocket::~NetSocket()
{
    LOG << "delete NetSocket sock_fd=" << m_socket << " (" << this << ")" << std::endl;

    if (m_socket != -1)
        close(m_socket);
}

//------------------------------------------------------------------------------

bool NetSocket::connect(unsigned int ip, unsigned int port, bool forceCoalesce)
{
    // Create socket handle
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        PLOG << "socket";
        return false;
    }

    // In this case turn off Nagle algorithm if desired
    if (!forceCoalesce) {
        int x = 1;
        if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (void *)&x, sizeof(x))) {
            PLOG << "setsockopt";
        }
    }

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(ip);
    sa.sin_port = htons(port);

    // Connect!
    if (::connect(m_socket, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
        PLOG << "connect";
        close(m_socket);
        m_socket = -1;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------

void NetSocket::setBlocking(bool blocking)
{
    unsigned long val = (blocking) ? 0 : 1;
    ioctl(m_socket, FIONBIO, &val);
}

//------------------------------------------------------------------------------

void NetSocket::send(std::shared_ptr<Packet> packet, bool clearTimeout)
{
    if (clearTimeout)
        m_timeOut = 0;

    // Just add to output list
    m_outList.push_back(packet);
}

//------------------------------------------------------------------------------

void NetSocket::handleOutput()
{
    // Ok. Send stuff.
    bool sent = false;
    do {
        // There must be something to send
        assert(!m_outList.empty());

        std::shared_ptr<Packet> packet = *(m_outList.begin());

        const char *data = packet->getData();
        int len = static_cast<int>(packet->getSize()); // cast from ulong!

        int rc = ::send(m_socket, data + m_sendOffset, len - m_sendOffset, MSG_NOSIGNAL);

        if (rc > 0) {
            // Log
            //LOG_PACKET(m_id, data + m_sendOffset, rc, "send");
            BaseSocketManager::singleton().addToOutbound(rc);

            m_sendOffset += rc; // number of bytes sent
            sent = true;
        } else if (errno == EWOULDBLOCK) { 
            sent = false;
        } else {
            LOG_WARNING << "::send returned: " << rc << std::endl;
            handleException();
            sent = false;
        }

        // Was whole packet sent?
        if (m_sendOffset == len) {
            // We are done with this packet
            m_outList.pop_front();
            m_sendOffset = 0;
        }
    } while (sent && !m_outList.empty());
}

//------------------------------------------------------------------------------

void NetSocket::handleInput()
{
    bool pktReceived = false;
    uint32 packetSize = 0;

    int rc = recv(m_socket, m_recvBuff + m_recvBegin + m_recvOffset, RECV_BUFFER_SIZE - (m_recvBegin + m_recvOffset), 0);

    if (rc == 0)
        return; // TODO: Check it out!  The return value will be 0 when the peer has performed an orderly shutdown.

    if (rc == -1) {
        m_deleteFlag = 1;
        return;
    }

    // Log
    //LOG_PACKET(m_id, m_recvBuff + m_recvBegin + m_recvOffset, rc, "recv");
    BaseSocketManager::singleton().addToInbound(rc);

    const unsigned int hdrSize = sizeof(uint32);
    // Data that was not processed
    unsigned int newData = m_recvOffset + rc;
    int processedData = 0;

    while (newData > hdrSize) {
        // There is packet size value on buffer begin position
        packetSize = *(reinterpret_cast<uint32*>(m_recvBuff + m_recvBegin));
        packetSize = ntohl(packetSize);

        // If so, we heed to wait for more data.
        if (newData < packetSize)
            break;

        if (packetSize > MAX_PACKET_SIZE) {
            LOG_FATAL << "Buffer overruns!";
            break;
        }

        if (newData >= packetSize) {
            // There is enough data to recreate packet
            std::shared_ptr<Packet> packet(new Packet(&m_recvBuff[m_recvBegin + hdrSize], packetSize - hdrSize));
            m_inList.push_back(packet);
            pktReceived = true;
            processedData += packetSize;
            newData -= packetSize;
            m_recvBegin += packetSize;
        }

        m_recvOffset = newData;

        if (pktReceived) {
            if (m_recvOffset == 0) {
                // Packets were processed and there are no partial data in buffer.
                // We can safely move begin position pointer to start of the buffer.
                m_recvBegin = 0;
            } else if (m_recvBegin + m_recvOffset + MAX_PACKET_SIZE > RECV_BUFFER_SIZE) {
                // There is risk of overruning the buffer.
                // We need to copy data to the start of the buffer.
                memcpy(m_recvBuff, &m_recvBuff[m_recvBegin], m_recvOffset);
            }
        }
    }
}

//------------------------------------------------------------------------------

void NetSocket::handleException()
{
    LOG_WARNING << "Exception in sockFd=" << m_socket << std::endl;
    m_deleteFlag |= 1;
}
