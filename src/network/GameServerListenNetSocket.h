#ifndef GAMESERVERLISTENNETSOCKET_H
#define GAMESERVERLISTENNETSOCKET_H

#include "ListenNetSocket.h"

class GameServerListenNetSocket : public ListenNetSocket
{
public:
    GameServerListenNetSocket(int portNum);

    void handleInput();
};

#endif // GAMESERVERLISTENNETSOCKET_H
