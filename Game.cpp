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

  if (!resMgr.addTexture("minecraft_tiles_big.png")) {
    std::cerr << "Unable to load texture.\n";
  }  

  m_map.loadFromFile("media/map1.tmx");

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
  m_map.drawLayer(e, "back");
  m_map.drawLayer(e, "collision");
  m_map.drawLayer(e, "ladders");

  for (GameObject* go : m_gameObjects) {
    go->draw(e);
  }

  m_map.drawLayer(e, "front");
}
