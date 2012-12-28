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

  m_x = 0.0f;
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

void Game::update(Engine */*e*/, float elapsedTime)
{
  //float v = 20.0f;
  //m_x += v * elapsedTime;
}

//------------------------------------------------------------------------------

void Game::draw(Engine *e)
{
  //e->drawSprite(*m_sprites[0]);
  glTranslatef(m_x, 0.0f, 0.0f);
  m_map.draw(e);
}
