#ifndef REMOTEEVENTSOCKET_H
#define REMOTEEVENTSOCKET_H

#include "NetSocket.h"

class RemoteEventSocket : public NetSocket
{
public:
    RemoteEventSocket();
    RemoteEventSocket(int socket, unsigned int ip);
};

#endif // REMOTEEVENTSOCKET_H
