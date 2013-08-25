#ifndef BASEGAMELOGIC_H
#define BASEGAMELOGIC_H

#include "GameLogic.h"
#include "PhysicsEngine.h"
#include "EventMgr.h"
#include "Map.h"

class BaseGameLogic : public GameLogic
{
public:
    BaseGameLogic();
    ~BaseGameLogic() override;

    PhysicsEngine *physicsEngine() { return m_physicsEngine; }

    void addGameObject(ActorCategory type,
                       const std::string& name,
                       float x, float y);

    void update(float elapsedTime) override;

protected:
    std::map<ActorId, ActorPtr> m_actors;
    PhysicsEngine *m_physicsEngine;

    float m_mapWidth;

private:
    bool isOnMap(ActorPtr actor);
};

#endif // BASEGAMELOGIC_H
