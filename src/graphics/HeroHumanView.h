#ifndef HEROHUMANVIEW_H
#define HEROHUMANVIEW_H

#include "HumanView.h"
#include "events/EventMgr.h"
#include "graphics/MapNode.h"
#include "input/KeyboardHandler.h"

#include <map>

class Renderer;
class SpriteNode;
class MapNode;

class HeroHumanView : public HumanView
{
    typedef HumanView super;

 public:
    HeroHumanView(const std::string& title, int screenWidth, int screenHeight,
                  bool screenFull = false);
    ~HeroHumanView() override;

    bool processInput(const SDL_Event& event) override;
    void update(float elapsedTime) override;
    void draw() override;

 private:
    void handleActorMoved(EventPtr event);
    void handleActorPhysicsStateChanged(EventPtr event);
    void handleActorCreated(EventPtr event);
    void handleActorDestroyed(EventPtr event);
    void handleInputCommand(EventPtr event);

    Renderer *m_renderer;
    ActorId m_heroId;

    EventListenerHelper elh;
    MapNode m_mapNode;

    std::map<ActorId, SpriteNode*> m_nodes;

    KeyboardHandler *m_keyboardHandler;
};

#endif // HEROHUMANVIEW_H
