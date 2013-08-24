/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Game.h"

#include "ResourceMgr.h"
#include "Engine.h"
#include "Actor.h"
#include "ActorFactory.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "components/RenderComponent.h"
#include "components/PhysicsComponent.h"
#include "Box2dPhysicsEngine.h"

#include <iostream>
#include <memory>
#include <utility> // pair

Game::Game()
{
    m_physicsEngine = new Box2dPhysicsEngine;
    m_physicsEngine->init();
      
    // Read map from file
    ResourceMgr& resMgr = ResourceMgr::singleton();
    resMgr.setDataFolder("media/");

    // Hero texture
    resMgr.addTexture("MegaMan_001.png");

    m_map.loadFromFile("media/map2.tmx");

    // Build objects
    std::vector<MapObject> mapObjects;
    m_map.getObjects(mapObjects);

    std::cout << "INFO: " << mapObjects.size() << " objects loaded.\n";

    for (const MapObject& obj : mapObjects) {
        ActorPtr a = ActorFactory::create(this, obj);
        m_actors.insert(std::make_pair(a->id(), a));
        if (obj.name == "hero")
            Engine::singleton().centerViewOn(obj.x, obj.y);
    }

    // Register event listeners
    EventListenerPtr el = EventMgr::makeListener(std::bind(&Game::handleActorCollidedEvent, this, std::placeholders::_1));
    EventMgr::singleton().addListener(ACTOR_COLLIDED, el);
    m_listeners.insert(std::make_pair(ACTOR_COLLIDED, el));

    EventListenerPtr el2 = EventMgr::makeListener(std::bind(&Game::handleActorPhysicsStateChanged, this, std::placeholders::_1));
    EventMgr::singleton().addListener(ACTOR_PHYSICS_STATE_CHANGED, el2);
    m_listeners.insert(std::make_pair(ACTOR_PHYSICS_STATE_CHANGED, el2));

    EventListenerPtr el3 = EventMgr::makeListener(std::bind(&Game::handleActorMoved, this, std::placeholders::_1));
    EventMgr::singleton().addListener(ACTOR_MOVED, el3);
    m_listeners.insert(std::make_pair(ACTOR_MOVED, el3));
}

//------------------------------------------------------------------------------

Game::~Game()
{
    auto it = std::begin(m_actors);
    while (it != std::end(m_actors)) {
        ActorPtr actor = it->second;

        it = m_actors.erase(it);
        // Must be called to break cyclic references
        actor->destroy();
    }

    // Unregister event listeners
    for (auto pair : m_listeners) {
        EventMgr::singleton().removeListener(pair.first, pair.second);
    }

    m_physicsEngine->shutdown();
    delete m_physicsEngine;
}

//------------------------------------------------------------------------------

void Game::initialize(Engine *e)
{
    // Set background color
    std::string bgColor = m_map.backgroundColor(); 
    if (!bgColor.empty()) {
        std::vector<int> color = hexColorToRgb(bgColor);
        e->setBackgroundColor(color[0], color[1], color[2]);
    }

    ScriptMgr::singleton().executeFile("map2_init.lua");
}

//------------------------------------------------------------------------------

bool Game::processInput(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_KEYUP:
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) return false;
        if (event.key.keysym.scancode == SDL_SCANCODE_G) toggleDrawDebug();
        break;
    }
    return true; // keep actoring
}

//------------------------------------------------------------------------------

void Game::update(Engine *e, float elapsedTime)
{
    m_physicsEngine->update(elapsedTime);

    for (const auto& pair : m_actors) {
        pair.second->update(e, elapsedTime);
    } 

    m_fpsCounter.update(elapsedTime);
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

void Game::draw(Engine *e)
{
    // Draw only visible part of the map
    float x1, x2, y1, y2;
    e->viewBounds(&x1, &x2, &y1, &y2);

    // TODO: read order from map
    m_map.drawLayer(e, "back",   x1, x2, y1, y2);
    m_map.drawLayer(e, "ground", x1, x2, y1, y2);
 
    // Draw actors
    for (const auto& pair : m_actors) {
        ActorPtr a = pair.second;
        // Get weak_ptr
        auto rcwp = a->getComponent<RenderComponent>(RENDER);
        // Try to get shared_ptr 
        if (auto rcsp = rcwp.lock()) {
            auto pcwp = a->getComponent<PhysicsComponent>(PHYSICS);
            if (auto pcsp = pcwp.lock()) {
                e->drawImage(rcsp->currentImage(),
                             pcsp->posX() + rcsp->xOffset(),
                             pcsp->posY() + rcsp->yOffset(),
                             pcsp->angle());
            }
        }
    }

    m_map.drawLayer(e, "water", x1, x2, y1, y2);
    m_map.drawLayer(e, "front", x1, x2, y1, y2);

    m_physicsEngine->drawDebugData();
}

//------------------------------------------------------------------------------

void Game::addGameObject(ActorCategory type, const std::string& name,
                         float x, float y)
{
    ActorPtr a = ActorFactory::create(this, type, name, x, y);
    m_actors.insert(std::make_pair(a->id(), a));        
}

//------------------------------------------------------------------------------

bool Game::isOnMap(ActorPtr a)
{
    auto pcwp = a->getComponent<PhysicsComponent>(PHYSICS);
    // Try to get shared_ptr 
    if (auto pcsp = pcwp.lock()) {
        float x = pcsp->posX();
        float y = pcsp->posY();
        if (x < 0.0f || x > m_map.width())
            return false;
        else if (y < 0.0f)
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------

void Game::handleActorCollidedEvent(EventPtr event)
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

//------------------------------------------------------------------------------

void Game::handleActorMoved(EventPtr event)
{
    auto e = std::static_pointer_cast<MoveEvent>(event);

    ActorPtr actor = m_actors[e->m_actor];

    if (actor->name() == "hero") {
        float x = e->m_x;
        float y = e->m_y;

        // Respect map borders
        float bLeft, bRight, bTop, bBottom;
        Engine::singleton().viewBounds(&bLeft, &bRight, &bBottom, &bTop);

        float hw = (bRight - bLeft) / 2.0f;
        float hh = (bTop - bBottom) / 2.0f;
        if (x < hw) x = hw;
        if (y < hh) y = hh;
        if (x > map()->width() - hw) x = map()->width() - hw;

        Engine::singleton().centerViewOn(x, y);
    }
}

//------------------------------------------------------------------------------

void Game::toggleDrawDebug()
{
    m_physicsEngine->toggleDrawDebug();
}

