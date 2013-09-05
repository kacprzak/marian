/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef NETSOCKET_H
#define NETSOCKET_H

#include "Packet.h"

#include <memory>
#include <list>

#define MAX_PACKET_SIZE 256
#define RECV_BUFFER_SIZE (MAX_PACKET_SIZE * 256)

class NetSocket
{
    friend class BaseSocketManager;

    typedef std::list<std::shared_ptr<Packet> > PacketList;

 public:
    NetSocket();
    NetSocket(int newSock, unsigned int hostIp);
    virtual ~NetSocket();

    bool connect(unsigned int ip, unsigned int port, bool forceCoalesce = false);
    void setBlocking(bool blocking);
    void send(std::shared_ptr<Packet> packet, bool clearTimeout = true);

    virtual int hasOutput() { return !m_outList.empty(); }
    virtual void handleOutput();
    virtual void handleInput();
    virtual void handleException() {}
    virtual void timeOut() { m_timeOut = 0; }

    int ipAdress() { return m_ipaddr; }

 protected:
    int m_socket;           ///< Socker file descriptor
    int m_id;               ///< Id given by manager

    int m_deleteFlag;

    PacketList m_outList;   ///< packet to send
    PacketList m_inList;    ///< packets received

    char m_recvBuff[RECV_BUFFER_SIZE];      ///< recieved data
    unsigned int m_recvOffset, m_recvBegin; ///< tracking buffer

    int m_sendOffset;       ///< tracking output buffer
    unsigned int m_timeOut; ///< when will the socket timeout
    unsigned int m_ipaddr;  ///< IP of remote connection

    int m_internal;         ///< remote IP internal or external
    int m_timeCreated;      ///< socket creation time

 private:
    void logHelper(const char *data, int size);
};

#endif // NETSOCKET_H
