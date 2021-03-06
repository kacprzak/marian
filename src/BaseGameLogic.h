#ifndef BASEGAMELOGIC_H
#define BASEGAMELOGIC_H

#include "GameLogic.h"
#include "Map.h"
#include "PhysicsEngine.h"
#include "events/EventMgr.h"

#include <memory>

class BaseGameLogic : public GameLogic
{
  public:
    BaseGameLogic();
    ~BaseGameLogic() override;

    PhysicsEngine* physicsEngine() override { return m_physicsEngine.get(); }

    void addGameObject(ActorCategory type, const std::string& name, float x,
                       float y);

    void update(float elapsedTime) override;

  protected:
    std::unique_ptr<PhysicsEngine> m_physicsEngine;
    std::map<ActorId, std::unique_ptr<Actor>> m_actors;
    float m_mapWidth;

  private:
    bool isOnMap(Actor& actor) const;
};

#endif // BASEGAMELOGIC_H
