#ifndef LISTENNETSOCKET_H
#define LISTENNETSOCKET_H

#include "NetSocket.h"

namespace net {

class ListenNetSocket : public NetSocket
{
  public:
    ListenNetSocket();
    explicit ListenNetSocket(int portNum)
        : port(0)
    {
        init(portNum);
    }

    void init(int portNum);
    int acceptConnection(unsigned int* addr);

    unsigned short port;
};

} // namespace network

#endif // LISTENNETSOCKET_H
