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

  static float v = 250.0f;

  if (m_keys[SDLK_RIGHT]) x += v * elapsedTime;
  if (m_keys[SDLK_LEFT])  x -= v * elapsedTime;
  if (m_keys[SDLK_UP])    y += v * elapsedTime;
  if (m_keys[SDLK_DOWN])  y -= v * elapsedTime;

  // Dont leave the map
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x > m_map.pixelSize().x - s.width())  x = m_map.pixelSize().x - s.width();
  if (y > m_map.pixelSize().y - s.height()) y = m_map.pixelSize().y - s.height();

  // Respect walls ;)
  // Hotspots
  Vector2<int> top   (x + s.width()/2, y + s.height());
  Vector2<int> bottom(x + s.width()/2, y);
  Vector2<int> left  (x              , y + s.height()/2);
  Vector2<int> right (x + s.width()  , y + s.height()/2);

  unsigned gid = m_map.getTileGidAt(left.x, left.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = m_map.getTileRectAt(left.x, left.y);
    x = tileRect.right();
  }

  gid = m_map.getTileGidAt(right.x, right.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = m_map.getTileRectAt(right.x, right.y);
    x = tileRect.left() - s.width();
  }

  gid = m_map.getTileGidAt(bottom.x, bottom.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = m_map.getTileRectAt(bottom.x, bottom.y);
    y = tileRect.top();
  }

  gid = m_map.getTileGidAt(top.x, top.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = m_map.getTileRectAt(top.x, top.y);
    y = tileRect.bottom() - s.height();
  }

  s.setPosition(x, y);

  e->centerOnPixel(s.position().x, s.position().y);
}

//------------------------------------------------------------------------------

void Game::draw(Engine *e)
{
  m_map.drawLayer(e, "back");
  m_map.drawLayer(e, "collision");

  for (const Sprite& s : m_sprites) {
    e->drawSprite(s);
  }

  m_map.drawLayer(e, "front");
}
