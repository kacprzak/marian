#include "GameLogicImpl.h"

#include "Box2dPhysicsEngine.h"
#include "Logger.h"
#include "Map.h"
#include "ResourceMgr.h"
#include "ScriptMgr.h"
#include "actors/Actor.h"
#include "actors/ActorFactory.h"

#include "components/PhysicsComponent.h"
#include "components/RenderComponent.h"

using namespace event;

GameLogicImpl::GameLogicImpl()
{
    Map map;

    // Read map from file
    const std::string& assetsFolder = ResourceMgr::singleton().dataFolder();
    map.loadFromFile(assetsFolder + "map2.tmx");

    m_mapWidth = map.width();

    // Build objects
    std::vector<MapObject> mapObjects;
    map.getObjects(mapObjects);

    // std::cout << "INFO: " << mapObjects.size() << " MapObjects loaded.\n";

    for (const MapObject& obj : mapObjects) {
        auto a = ActorFactory::create(this, obj);
        m_actors.insert(std::make_pair(a->id(), std::move(a)));
    }

    // Register event listeners
    elh.registerListener(ACTOR_COLLIDED,
                         std::bind(&GameLogicImpl::handleActorCollided, this,
                                   std::placeholders::_1));
    // elh.registerListener(ACTOR_PHYSICS_STATE_CHANGED,
    //                     std::bind(&Game::handleActorPhysicsStateChanged,
    //                               this, std::placeholders::_1));
    elh.registerListener(INPUT_COMMAND,
                         std::bind(&GameLogicImpl::handleInputCommand, this,
                                   std::placeholders::_1));
}

//------------------------------------------------------------------------------

void GameLogicImpl::onBeforeMainLoop(Engine* /*e*/)
{
    ScriptMgr::singleton().executeFile("map2_init.lua");
}

//------------------------------------------------------------------------------

void GameLogicImpl::update(float elapsedTime)
{
    BaseGameLogic::update(elapsedTime);
}

//------------------------------------------------------------------------------

void GameLogicImpl::handleActorCollided(Event& event)
{
    CollisionEvent& e = static_cast<CollisionEvent&>(event);

    const auto& a               = m_actors.at(e.m_actorA);
    const auto& b               = m_actors.at(e.m_actorB);
    CollisionEvent::Phase phase = e.m_phase;

    auto pcqpA = a->getComponent<PhysicsComponent>(PHYSICS);
    if (auto pcsp = pcqpA.lock()) {
        if (phase == CollisionEvent::BEGIN)
            pcsp->handleBeginContact(*b, e.m_actorALimbData);
        else
            pcsp->handleEndContact(*b, e.m_actorALimbData);
    }

    auto pcqpB = b->getComponent<PhysicsComponent>(PHYSICS);
    if (auto pcsp = pcqpB.lock()) {
        if (phase == CollisionEvent::BEGIN)
            pcsp->handleBeginContact(*a, e.m_actorBLimbData);
        else
            pcsp->handleEndContact(*a, e.m_actorBLimbData);
    }
}

//------------------------------------------------------------------------------

void GameLogicImpl::handleInputCommand(Event& event)
{
    ActorInputEvent& e = static_cast<ActorInputEvent&>(event);

    auto it = m_actors.find(e.m_actorId);

    if (it == std::end(m_actors)) {
        LOG << "There is no actor with id " << e.m_actorId << '\n';
        return;
    }

    const auto& a = it->second;

    if (a && (a->category() == HERO)) {
        auto hpcw = a->getComponent<PhysicsComponent>(PHYSICS);
        if (auto hpcs = hpcw.lock()) {
            hpcs->handleInputCommand(e.m_command);
        }
    }
}

//------------------------------------------------------------------------------
