/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Game.h"

#include "ResourceMgr.h"
#include "Engine.h"
#include "Actor.h"
#include "ActorFactory.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "Box2dPhysicsEngine.h"
#include "EventManager.h"

#include <iostream>
#include <memory>

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
        if (event.key.keysym.sym == SDLK_ESCAPE) return false;
        if (event.key.keysym.sym == SDLK_g) toggleDrawDebug();
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

    EventManager::singleton().update();
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
            rcsp->draw(e);
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

void Game::toggleDrawDebug()
{
    m_physicsEngine->toggleDrawDebug();
}

