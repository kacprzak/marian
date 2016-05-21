/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAMESERVERLISTENNETSOCKET_H
#define GAMESERVERLISTENNETSOCKET_H

#include "ListenNetSocket.h"

namespace net {

    class GameServerListenNetSocket : public ListenNetSocket
    {
    public:
        explicit GameServerListenNetSocket(int portNum);

        void handleInput();
    };

} // namespace network

#endif // GAMESERVERLISTENNETSOCKET_H
