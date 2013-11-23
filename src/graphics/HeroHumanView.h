#ifndef HEROHUMANVIEW_H
#define HEROHUMANVIEW_H

#include "HumanView.h"
#include "events/EventMgr.h"
#include "graphics/MapNode.h"
#include "input/HeroController.h"

#include <memory>
#include <map>

namespace gfx {

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
    void handleActorMoved(event::EventPtr event);
    void handleActorPhysicsStateChanged(event::EventPtr event);
    void handleActorCreated(event::EventPtr event);
    void handleActorDestroyed(event::EventPtr event);
    void handleInputCommand(event::EventPtr event);

    std::unique_ptr<Renderer> m_renderer;
    ActorId m_heroId;

    event::EventListenerHelper elh;
    MapNode m_mapNode;

    std::map<ActorId, SpriteNode*> m_nodes;

    std::unique_ptr<HeroController> m_keyboardHandler;
};

} // namespace gfx

#endif // HEROHUMANVIEW_H
