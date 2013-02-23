/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Game.h"

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
        if (obj.name == "hero") {
            // Tile based GameObject
            //std::string imageSource = m_map.imageForTile(obj.gid);
            //const Texture *tex = ResourceMgr::instance().getTexture(imageSource);     
      
            //Sprite sprite(tex, m_map.rectForTile(obj.gid));     
            //std::cout << "INFO: " << sprite << '\n';
      
            GameObject *gameObject = new Hero(e, this,
                                              b2Vec2(obj.x + obj.width/2,
                                                     obj.y + obj.width/2),
                                              b2Vec2(obj.width, obj.height));
            m_gameObjects.push_back(gameObject);

        } else {
            // Static collision shape
            b2BodyDef groundBodyDef;
            groundBodyDef.position.Set(obj.x, obj.y);

            b2Body *groundBody = e->world()->CreateBody(&groundBodyDef);


            const std::string& shape = obj.shape;
            if (shape == "polyline" || shape == "polygon") {
                size_t numOfPoints = obj.points.size();
                b2Vec2 vs[numOfPoints];
                
                for (size_t i = 0; i < numOfPoints; ++i) {
                    auto& p = obj.points[i];
                    vs[i].Set(p.first, p.second);
                }

                b2ChainShape chain;
                if (obj.shape == "polyline")
                    chain.CreateChain(vs, numOfPoints);
                else
                    chain.CreateLoop(vs, numOfPoints);

                groundBody->CreateFixture(&chain, 0.0f);

            } else {
                // Rect object
                b2PolygonShape box;
                float hw = obj.width / 2;
                float hh = obj.height / 2;
                box.SetAsBox(hw, hh, b2Vec2(hw, hh), 0.0f);

                groundBody->CreateFixture(&box, 0.0f);
            }
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
    // Draw only visible part of the map
    float x1, x2, y1, y2;
    e->viewBounds(&x1, &x2, &y1, &y2);

    // TODO: read order from map
    m_map.drawLayer(e, "sky",       x1, x2, y1, y2);
    m_map.drawLayer(e, "back",      x1, x2, y1, y2);
    m_map.drawLayer(e, "collision", x1, x2, y1, y2);
    m_map.drawLayer(e, "ladders",   x1, x2, y1, y2);

    for (GameObject* go : m_gameObjects) {
        go->draw(e);
    }

    m_map.drawLayer(e, "water", x1, x2, y1, y2);
    m_map.drawLayer(e, "front", x1, x2, y1, y2);
}
