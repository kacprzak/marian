/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef REMOTEEVENTSOCKET_H
#define REMOTEEVENTSOCKET_H

#include "NetSocket.h"

namespace net {

    class RemoteEventSocket : public NetSocket
    {
        using super = NetSocket;

    public:
        RemoteEventSocket();
        RemoteEventSocket(int socket, unsigned int ip);

        // NetSocket interface
        void handleInput();
    };

} // namespace network

#endif // REMOTEEVENTSOCKET_H
