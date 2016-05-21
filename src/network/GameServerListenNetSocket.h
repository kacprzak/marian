/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAMESERVERLISTENNETSOCKET_H
#define GAMESERVERLISTENNETSOCKET_H

#include "ListenNetSocket.h"

namespace net {

    class GameServerListenNetSocket : public ListenNetSocket
    {
        public:
        GameServerListenNetSocket(int portNum);

        void handleInput();
    };

} // namespace network

#endif // GAMESERVERLISTENNETSOCKET_H
