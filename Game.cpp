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

  const Texture *tex = resMgr.getTexture("minecraft_tiles_big.png");

  Sprite *sprite = new Sprite(tex);
  m_sprites.push_back(sprite);
}

//------------------------------------------------------------------------------

Game::~Game()
{
  delete m_sprites[0];
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
  Sprite *s = m_sprites[0];
  
  float v = 20.0f;
  
  float x = s->position().x;
  float y = s->position().y;
  
  float new_x = x + v * elapsedTime;
  
  s->setPosition(new_x, y);
}

//------------------------------------------------------------------------------

void Game::draw(Engine *e)
{
  e->drawSprite(*m_sprites[0]);
}
