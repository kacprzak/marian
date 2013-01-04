#include "Game.h"

#include "Sprite.h"
#include "ResourceMgr.h"
#include "Engine.h"
#include "GameObject.h"
#include "Hero.h"

#include <iostream>

Game::Game()
{
  ResourceMgr& resMgr = ResourceMgr::instance();
  resMgr.setDataFolder("media/");

  m_map.loadFromFile("media/map2.tmx");

  std::vector<Sprite> sprites;
  m_map.getObjects(sprites);

  std::cout << "INFO: " << sprites.size() << " objects loaded.\n";

  for (const Sprite& s : sprites) {
    std::cout << "INFO: " << s << '\n';

    GameObject *gameObject = new Hero(this, s);
    m_gameObjects.push_back(gameObject);
  }
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
  int mapPixHeight   = m_map.pixelSize().y;

  int xFrom = (pos.x - e->screenWidth()  / 2) / m_map.tileWidth();
  int xTo   = (pos.x + e->screenWidth()  / 2) / m_map.tileWidth() + 2;
  int yFrom = (mapPixHeight - pos.y - e->screenHeight() / 2) / m_map.tileHeight() - 1;
  int yTo   = (mapPixHeight - pos.y + e->screenHeight() / 2) / m_map.tileHeight() + 1;

  // Clamp coords
  Vector2<int> mapSize = m_map.tileSize();
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
