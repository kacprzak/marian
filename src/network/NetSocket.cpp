/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "NetSocket.h"

#include <unistd.h> // close()
#include <cstdio>
#include <sys/ioctl.h>

NetSocket::NetSocket()
{
    m_socket = -1;
    m_deleteFlag = 0;
    m_sendOfs = 0;
    m_timeOut = 0;
    m_recvOfs = m_recvBegin = 0;
    m_internal = 0;
    //m_bBinaryProtocol = 1;
}

//------------------------------------------------------------------------------

NetSocket::~NetSocket()
{
    if (m_socket != -1)
        close(m_socket);
}

//------------------------------------------------------------------------------

bool NetSocket::connect(unsigned int ip, unsigned int port, bool forceCoalesce)
{
    // Create socket handle
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        perror("socket");
        return false;
    }

    if (forceCoalesce) {
        // TODO
    }

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(ip);
    sa.sin_port = htons(port);

    // Connect!
    if (::connect(m_socket, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
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
    bool fSent = false;
    do {
        // There must be something to send
        assert(!m_outList.empty());

        std::shared_ptr<Packet> packet = *(m_outList.begin());

        const char *data = packet->getData();
        int len = static_cast<int>(packet->getSize()); // cast from ulong!

        int rc = ::send(m_socket, data + m_sendOfs, len - m_sendOfs, 0);
        if (rc > 0) {
            m_sendOfs += rc; // number of bytes sent
            fSent = true;
        } else {
            // TODO: Something went wrong!
            fSent = false;
        }

        // Was whole packet sent?
        if (m_sendOfs == len) {
            // We are done with this packet
            m_outList.pop_front();
            m_sendOfs = 0;
        }
    } while (fSent && !m_outList.empty());
}

//------------------------------------------------------------------------------

void NetSocket::handleInput()
{
    // TODO
}
