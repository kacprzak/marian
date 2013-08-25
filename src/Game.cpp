/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Game.h"

#include "Actor.h"
#include "ActorFactory.h"
#include "ScriptMgr.h"
#include "components/RenderComponent.h"
#include "components/PhysicsComponent.h"
#include "Box2dPhysicsEngine.h"

#include "Map.h"
#include "ResourceMgr.h"

Game::Game()
{
    Map map;

    // Read map from file
    map.loadFromFile("media/map2.tmx");

    m_mapWidth = map.width();

#if 1 // This should be in View!!!
    // Hero texture
    ResourceMgr::singleton().addTexture("MegaMan_001.png");

    // Load map images
    auto images = map.externalImages();
    for (const std::string& image : images)
        ResourceMgr::singleton().addTexture(image);
#endif

    // Build objects
    std::vector<MapObject> mapObjects;
    map.getObjects(mapObjects);

    //std::cout << "INFO: " << mapObjects.size() << " MapObjects loaded.\n";

    for (const MapObject& obj : mapObjects) {
        ActorPtr a = ActorFactory::create(this, obj);
        m_actors.insert(std::make_pair(a->id(), a));
    }

    // Register event listeners
    elh.registerListener(ACTOR_COLLIDED, std::bind(&Game::handleActorCollided, this, std::placeholders::_1));
    elh.registerListener(ACTOR_PHYSICS_STATE_CHANGED, std::bind(&Game::handleActorPhysicsStateChanged, this, std::placeholders::_1));
}

//------------------------------------------------------------------------------

void Game::initialize(Engine * /*e*/)
{
    ScriptMgr::singleton().executeFile("map2_init.lua");
}

//------------------------------------------------------------------------------

void Game::update(float elapsedTime)
{
    BaseGameLogic::update(elapsedTime);

    m_fpsCounter.update(elapsedTime);
}

//------------------------------------------------------------------------------

void Game::handleActorCollided(EventPtr event)
{
    std::shared_ptr<CollisionEvent> e = std::static_pointer_cast<CollisionEvent>(event);

    ActorPtr a = m_actors[e->m_actorA];
    ActorPtr b = m_actors[e->m_actorB];
    CollisionEvent::Phase phase = e->m_phase;

    auto pcqpA = a->getComponent<PhysicsComponent>(PHYSICS);
    if (auto pcsp = pcqpA.lock()) {
        if (phase == CollisionEvent::BEGIN)
            pcsp->handleBeginContact(b.get(), e->m_actorALimbData);
        else
            pcsp->handleEndContact(b.get(), e->m_actorALimbData);
    }

    auto pcqpB = b->getComponent<PhysicsComponent>(PHYSICS);
    if (auto pcsp = pcqpB.lock()) {
        if (phase == CollisionEvent::BEGIN)
            pcsp->handleBeginContact(a.get(), e->m_actorBLimbData);
        else
            pcsp->handleEndContact(a.get(), e->m_actorBLimbData);
    }
}

//------------------------------------------------------------------------------

void Game::handleActorPhysicsStateChanged(EventPtr event)
{
    std::shared_ptr<PhysicsStateChangeEvent> e = std::static_pointer_cast<PhysicsStateChangeEvent>(event);

    ActorPtr actor = m_actors[e->m_actor];
    ActorPhysicsStateId state = e->m_newState;

    // RenderComponent weak ptr
    auto rcwp = actor->getComponent<RenderComponent>(RENDER);

    if (auto rcsp = rcwp.lock()) {
        rcsp->changePhysicsState(state);
    }
}
