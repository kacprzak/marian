#include "Game.h"

#include "Sprite.h"
#include "ResourceMgr.h"
#include "Engine.h"

#include <iostream>

Game::Game()
{
  for (int i = 0; i < SDLK_LAST; ++i) {
    m_keys[i] = false;
  }

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
  //
}

//------------------------------------------------------------------------------

bool Game::processInput(const SDL_Event& event)
{
  switch (event.type) {
  case SDL_KEYUP:
    m_keys[event.key.keysym.sym] = false;
    //std::cout << "Key Up: " << event.key.keysym.sym << std::endl;
    break;
  case SDL_KEYDOWN:
    m_keys[event.key.keysym.sym] = true;
    if (m_keys[SDLK_ESCAPE]) return false;
    //std::cout << "Key Down: " << event.key.keysym.sym << std::endl;
    break;
  }
  return true; // keep going
}

//------------------------------------------------------------------------------

void Game::update(Engine *e, float elapsedTime)
{
  Sprite& s = m_sprites[0];

  float x = s.position().x;
  float y = s.position().y;

  static float v = 100.0f;

  if (m_keys[SDLK_RIGHT]) x += v * elapsedTime;
  if (m_keys[SDLK_LEFT])  x -= v * elapsedTime;
  if (m_keys[SDLK_UP])    y += v * elapsedTime;
  if (m_keys[SDLK_DOWN])  y -= v * elapsedTime;

  s.setPosition(x, y);

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
