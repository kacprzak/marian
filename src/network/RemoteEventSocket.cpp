#include "RemoteEventSocket.h"

RemoteEventSocket::RemoteEventSocket()
{

}

//------------------------------------------------------------------------------

RemoteEventSocket::RemoteEventSocket(int socket, unsigned int ip)
    : NetSocket(socket, ip)
{

}
