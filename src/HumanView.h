#ifndef HUMANVIEW_H
#define HUMANVIEW_H

#include "GameView.h"
#include "events/EventMgr.h"
#include "graphics/MapNode.h"
#include "input/KeyboardHandler.h"

#include <map>

class SpriteNode;
class MapNode;

class HumanView : public GameView
{
 public:
    HumanView();
    ~HumanView();

    void initialize(Engine *e) override;

    bool processInput(const SDL_Event& event) override;
    void update(float elapsedTime) override;
    void draw(Engine *e) override;

 private:
    void handleActorMoved(EventPtr event);
    void handleActorPhysicsStateChanged(EventPtr event);
    void handleActorCreated(EventPtr event);
    void handleActorDestroyed(EventPtr event);
    void handleInputCommand(EventPtr event);

    ActorId m_heroId;

    EventListenerHelper elh;
    MapNode m_mapNode;

    std::map<ActorId, SpriteNode*> m_nodes;

    KeyboardHandler *m_keyboardHandler;
};

#endif // HUMANVIEW_H
