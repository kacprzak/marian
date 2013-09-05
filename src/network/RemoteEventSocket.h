#ifndef REMOTEEVENTSOCKET_H
#define REMOTEEVENTSOCKET_H

#include "NetSocket.h"

class RemoteEventSocket : public NetSocket
{
    typedef NetSocket super;

public:
    RemoteEventSocket();
    RemoteEventSocket(int socket, unsigned int ip);

    // NetSocket interface
    void handleInput();
};

#endif // REMOTEEVENTSOCKET_H
