#ifndef REMOTEGAMELOGIC_H
#define REMOTEGAMELOGIC_H

#include "GameLogic.h"

#include "events/EventMgr.h"
#include "NetworkEventForwarder.h"

class RemoteGameLogic : public GameLogic
{
public:
    RemoteGameLogic(int socketId);

    // GameLogic interface
    void update(float elapsedTime);

protected:
    int m_socketId;
    NetworkEventForwarder m_nef;

 private:
    EventListenerHelper elh;
};

#endif // REMOTEGAMELOGIC_H
