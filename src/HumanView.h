#ifndef HUMANVIEW_H
#define HUMANVIEW_H

#include "GameView.h"
#include "Map.h"
#include "EventMgr.h"

class HumanView : public GameView
{
 public:
    HumanView();

    void initialize(Engine *e) override;

    bool processInput(const SDL_Event& event) override;
    void update(float elapsedTime) override;
    void draw(Engine *e) override;

 private:
    void handleActorMoved(EventPtr event);

    EventListenerHelper elh;
    Map m_map;
};

#endif // HUMANVIEW_H
