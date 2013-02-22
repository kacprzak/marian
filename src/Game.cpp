/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Game.h"

#include "Sprite.h"
#include "ResourceMgr.h"
#include "Engine.h"
#include "GameObject.h"
#include "Hero.h"

#include <iostream>

Game::Game()
{
}

//------------------------------------------------------------------------------

void Game::initialize(Engine *e)
{
    ResourceMgr& resMgr = ResourceMgr::instance();
    resMgr.setDataFolder("media/");

    m_map.loadFromFile("media/map2.tmx");

    std::vector<MapObject> mapObjects;
    m_map.getObjects(mapObjects);

    std::cout << "INFO: " << mapObjects.size() << " objects loaded.\n";

    for (const MapObject& obj : mapObjects) {
        if (obj.gid) {
            std::string imageSource = m_map.imageForTile(obj.gid);
            const Texture *tex = ResourceMgr::instance().getTexture(imageSource);     
      
            Sprite sprite(tex, m_map.rectForTile(obj.gid));
      
            std::cout << "INFO: " << sprite << '\n';
      
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(obj.x, obj.y);
            b2Body* body = e->world()->CreateBody(&bodyDef);

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0.0f);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;

            body->CreateFixture(&fixtureDef);

            GameObject *gameObject = new Hero(this, sprite, body);
            m_gameObjects.push_back(gameObject);
        } else {
            std::cout << "INFO: " << obj.shape << '\n';
            std::cout << "INFO: {";
            for (auto& p : obj.points)
                std::cout << " " << p.first << "," << p.second << ' ';
            std::cout << "}\n";

            size_t numOfPoints = obj.points.size();
            b2Vec2 vs[numOfPoints];
            for (size_t i = 0; i < numOfPoints; ++i) {
                auto& p = obj.points[i];
                vs[i].Set(p.first, p.second);
            }

            b2ChainShape chain;
            if (obj.shape == "polyline")
                chain.CreateChain(vs, numOfPoints);
            else if (obj.shape == "polygon")
                chain.CreateLoop(vs, numOfPoints);

            b2BodyDef groundBodyDef;
            groundBodyDef.position.Set(obj.x, obj.y);

            b2Body *groundBody = e->world()->CreateBody(&groundBodyDef);
            groundBody->CreateFixture(&chain, 0.0f);
        }
    }
}

//------------------------------------------------------------------------------

void Game::clear(Engine * /*e*/)
{
    //
}

//------------------------------------------------------------------------------

Game::~Game()
{
    for (GameObject *go : m_gameObjects) {
        delete go;
    }
}

//------------------------------------------------------------------------------

bool Game::processInput(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_KEYUP:
        //std::cout << "Key Up: " << event.key.keysym.sym << std::endl;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) return false;
        //std::cout << "Key Down: " << event.key.keysym.sym << std::endl;
        break;
    }
    return true; // keep going
}

//------------------------------------------------------------------------------

void Game::update(Engine *e, float elapsedTime)
{
    for (GameObject *go : m_gameObjects) {
        go->update(e, elapsedTime);
    } 

    m_fpsCounter.update(elapsedTime);
}

//------------------------------------------------------------------------------

void Game::draw(Engine *e)
{
    // Hero pos
    Vector2<float> pos = m_gameObjects[0]->position();

    // Draw only visible part of the map
    float mapHeight   = m_map.size().y;

    float xFrom = pos.x - e->screenWidth()  / 2;
    float xTo   = pos.x + e->screenWidth()  / 2;
    float yFrom = mapHeight - pos.y - e->screenHeight() / 2;
    float yTo   = mapHeight - pos.y + e->screenHeight() / 2;

    // Clamp coords
    Vector2<float> mapSize = m_map.size();
    if (xFrom < 0) xFrom = 0;
    if (xTo   > mapSize.x) xTo = mapSize.x;
    if (yFrom < 0) yFrom = 0;
    if (yTo   > mapSize.y) yTo = mapSize.y;

    // TODO: read order from map
    m_map.drawLayer(e, "sky",       xFrom, xTo, yFrom, yTo);
    m_map.drawLayer(e, "back",      xFrom, xTo, yFrom, yTo);
    m_map.drawLayer(e, "collision", xFrom, xTo, yFrom, yTo);
    m_map.drawLayer(e, "ladders",   xFrom, xTo, yFrom, yTo);

    for (GameObject* go : m_gameObjects) {
        go->draw(e);
    }

    m_map.drawLayer(e, "water", xFrom, xTo, yFrom, yTo);
    m_map.drawLayer(e, "front", xFrom, xTo, yFrom, yTo);
}
