#ifndef LISTENNETSOCKET_H
#define LISTENNETSOCKET_H

#include "NetSocket.h"

class ListenNetSocket : public NetSocket
{
public:
    ListenNetSocket();
    ListenNetSocket(int portNum) : port(0) { init(portNum); }

    void init(int portNum);
    int acceptConnection(unsigned int *addr);

    unsigned short port;
};

#endif // LISTENNETSOCKET_H
