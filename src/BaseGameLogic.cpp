#include "BaseGameLogic.h"

#include "Box2dPhysicsEngine.h"
#include "Logger.h"
#include "actors/ActorFactory.h"
#include "events/EventMgr.h"

BaseGameLogic::BaseGameLogic()
    : m_physicsEngine(new Box2dPhysicsEngine)
    , m_mapWidth(0.0f)
{
    LOG << "created BaseGameLogic\n";
}

//------------------------------------------------------------------------------

BaseGameLogic::~BaseGameLogic() { LOG << "destroyed BaseGameLogic\n"; }

//------------------------------------------------------------------------------

void BaseGameLogic::update(float elapsedTime)
{
    using namespace event;

    m_physicsEngine->update(elapsedTime);

    for (const auto& pair : m_actors) {
        pair.second->update(elapsedTime);
    }

    EventMgr& evtMgr = EventMgr::singleton();
    evtMgr.update();

    // Remove dead GameObjects
    auto it = std::begin(m_actors);
    while (it != std::end(m_actors)) {
        const auto& actor = it->second;
        if (actor->dead()) {
            // Emit event
            evtMgr.queueEvent(
                std::make_unique<ActorDestroyedEvent>(actor->id()));
            it = m_actors.erase(it);
        } else {
            // Kill it if out of map
            if (!isOnMap(*actor))
                actor->die();
            ++it;
        }
    }
}

//------------------------------------------------------------------------------

void BaseGameLogic::addGameObject(ActorCategory type, const std::string& name,
                                  float x, float y)
{
    using namespace event;

    auto a = ActorFactory::create(this, type, name, x, y);
    auto actorId = a->id();
    m_actors.insert(std::make_pair(actorId, std::move(a)));

    // Emit event
    EventMgr::singleton().queueEvent(
        std::make_unique<ActorCreatedEvent>(actorId, type, x, y));
}

//------------------------------------------------------------------------------

bool BaseGameLogic::isOnMap(Actor& a) const
{
    const auto& pcwp = a.getComponent<PhysicsComponent>(PHYSICS);
    // Try to get shared_ptr
    if (auto pcsp = pcwp.lock()) {
        float x = pcsp->posX();
        float y = pcsp->posY();
        if (x < 0.0f || x > m_mapWidth || y < 0.0f)
            return false;
    }

    return true;
}
