#ifndef NETWORKEVENTFORWARDER_H
#define NETWORKEVENTFORWARDER_H

#include "../events/Event.h"

namespace net {

class NetworkEventForwarder
{
  public:
    explicit NetworkEventForwarder(int socketId);

    void forwardEvent(event::Event& e);

  protected:
    int m_socketId;
};

} // namespace network

#endif // NETWORKEVENTFORWARDER_H
