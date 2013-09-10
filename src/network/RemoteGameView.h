#ifndef REMOTEGAMEVIEW_H
#define REMOTEGAMEVIEW_H

#include "GameView.h"
#include "events/EventMgr.h"
#include "NetworkEventForwarder.h"

class RemoteGameView : public GameView
{
 public:
    RemoteGameView(int socketId);

    // GameView interface
    bool processInput(const SDL_Event &/*event*/) { return true; }
    void update(float /*elapsedTime*/) {}
    void draw(Engine *) {}
    void initialize(Engine *) {}
    void cleanup(Engine *) {}
    void onAttach(int gameViewId, unsigned long actorId)
    {
        m_viewId = gameViewId;
        m_actorId = actorId;
    }

 protected:
    unsigned long m_actorId;
    int m_viewId;
    int m_socketId;
    NetworkEventForwarder m_nef;

 private:
    EventListenerHelper elh;
};

#endif // REMOTEGAMEVIEW_H
