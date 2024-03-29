#ifndef REMOTEGAMEVIEW_H
#define REMOTEGAMEVIEW_H

#include "../GameView.h"
#include "NetworkEventForwarder.h"
#include "../events/EventMgr.h"

namespace net {

class RemoteGameView : public GameView
{
  public:
    explicit RemoteGameView(int socketId);

    // GameView interface
    bool processInput(const SDL_Event& /*event*/) { return true; }
    void update(float /*elapsedTime*/) {}
    void draw() {}
    void onAttach(int gameViewId, unsigned long actorId)
    {
        m_viewId  = gameViewId;
        m_actorId = actorId;
    }

  protected:
    unsigned long m_actorId;
    int m_viewId;
    int m_socketId;
    NetworkEventForwarder m_nef;

  private:
    event::EventListenerHelper elh;
};

} // namespace network

#endif // REMOTEGAMEVIEW_H
