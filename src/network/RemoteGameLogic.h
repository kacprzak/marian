/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef REMOTEGAMELOGIC_H
#define REMOTEGAMELOGIC_H

#include "GameLogic.h"

#include "events/EventMgr.h"
#include "NetworkEventForwarder.h"

namespace net {

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
    event::EventListenerHelper elh;
};

} // namespace network

#endif // REMOTEGAMELOGIC_H
