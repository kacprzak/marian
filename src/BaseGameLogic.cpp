#include "BaseGameLogic.h"

#include "ActorFactory.h"
#include "EventMgr.h"
#include "Box2dPhysicsEngine.h"

BaseGameLogic::BaseGameLogic()
    : m_mapWidth(0.0f)
{
    m_physicsEngine = new Box2dPhysicsEngine;
    m_physicsEngine->init();
}

//------------------------------------------------------------------------------

BaseGameLogic::~BaseGameLogic()
{
    auto it = std::begin(m_actors);
    while (it != std::end(m_actors)) {
        ActorPtr actor = it->second;

        it = m_actors.erase(it);
        // Must be called to break cyclic references
        actor->destroy();
    }

    m_physicsEngine->shutdown();
    delete m_physicsEngine;
}

//------------------------------------------------------------------------------

void BaseGameLogic::update(float elapsedTime)
{
    m_physicsEngine->update(elapsedTime);

    for (const auto& pair : m_actors) {
        pair.second->update(elapsedTime);
    }

    EventMgr::singleton().update();

    // Remove dead GameObjects
    auto it = std::begin(m_actors);
    while (it != std::end(m_actors)) {
        ActorPtr actor = it->second;
        if (actor->dead()) {
            it = m_actors.erase(it);
            // Must be called to break cyclic references
            actor->destroy();
        } else {
            // Kill it if out of map
            if (!isOnMap(actor))
                actor->die();
            ++it;
        }
    }
}

//------------------------------------------------------------------------------

void BaseGameLogic::addGameObject(ActorCategory type, const std::string& name,
                         float x, float y)
{
    ActorPtr a = ActorFactory::create(this, type, name, x, y);
    m_actors.insert(std::make_pair(a->id(), a));
}

//------------------------------------------------------------------------------

bool BaseGameLogic::isOnMap(ActorPtr a)
{
    auto pcwp = a->getComponent<PhysicsComponent>(PHYSICS);
    // Try to get shared_ptr
    if (auto pcsp = pcwp.lock()) {
        float x = pcsp->posX();
        float y = pcsp->posY();
        if (x < 0.0f || x > m_mapWidth)
            return false;
        else if (y < 0.0f)
            return false;
    }

    return true;
}