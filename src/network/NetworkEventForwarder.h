#ifndef NETWORKEVENTFORWARDER_H
#define NETWORKEVENTFORWARDER_H

#include "events/Event.h"

class NetworkEventForwarder
{
 public:
    NetworkEventForwarder(int socketId);

    void forwardEvent(EventPtr e);

 protected:
    int m_socketId;
};

#endif // NETWORKEVENTFORWARDER_H
