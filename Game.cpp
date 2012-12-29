#include "Game.h"

#include "Sprite.h"
#include "ResourceMgr.h"
#include "Engine.h"

#include <iostream>

Game::Game()
{
  ResourceMgr& resMgr = ResourceMgr::instance();
  resMgr.setDataFolder("media/");

  if (!resMgr.addTexture("minecraft_tiles_big.png")) {
    std::cerr << "Unable to load texture.\n";
  }  

  m_map.loadFromFile("media/map1.tmx");

  m_map.getObjects(m_sprites);
  std::cout << "INFO: " << m_sprites.size() << " objects loaded.\n";
  for (const Sprite& s : m_sprites) {
    std::cout << "INFO: " << s.toString() << '\n';
  }
}

//------------------------------------------------------------------------------

Game::~Game()
{
  //delete m_sprites[0];
}

//------------------------------------------------------------------------------

void Game::processInput(const SDL_Event& event)
{
  switch (event.type) {
  case SDL_KEYUP:
    std::cout << "Key Up: " << event.key.keysym.sym << std::endl;
    break;
  case SDL_KEYDOWN:
    std::cout << "Key Down: " << event.key.keysym.sym << std::endl;
    break;
  }
}

//------------------------------------------------------------------------------

void Game::update(Engine *e, float elapsedTime)
{
  const Sprite& s = m_sprites[0];

  e->centerOnPixel(s.position().x, s.position().y);
}

//------------------------------------------------------------------------------

void Game::draw(Engine *e)
{
  m_map.draw(e);

  for (const Sprite& s : m_sprites) {
    e->drawSprite(s);
  }
}
