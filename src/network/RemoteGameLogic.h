#ifndef REMOTEGAMELOGIC_H
#define REMOTEGAMELOGIC_H

#include "GameLogic.h"

#include "NetworkEventForwarder.h"
#include "events/EventMgr.h"

namespace net {

class RemoteGameLogic : public GameLogic
{
  public:
    explicit RemoteGameLogic(int socketId);

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
