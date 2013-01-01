#include "Hero.h"

#include "Engine.h"
#include "Game.h"
#include <iostream>

void Hero::update(Engine *e, float elapsedTime)
{
  Sprite& s = m_sprite;
  Map *map = m_game->map();

  float x = s.position().x;
  float y = s.position().y;

  static float v = 250.0f;

  if (e->isPressed(SDLK_RIGHT)) x += v * elapsedTime;
  if (e->isPressed(SDLK_LEFT))  x -= v * elapsedTime;
  if (e->isPressed(SDLK_UP))    y += v * elapsedTime;
  if (e->isPressed(SDLK_DOWN))  y -= v * elapsedTime;

  // Dont leave the map
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x > map->pixelSize().x - s.width())  x = map->pixelSize().x - s.width();
  if (y > map->pixelSize().y - s.height()) y = map->pixelSize().y - s.height();

  // Respect walls ;)
  // Hotspots
  Vector2<int> top   (x + s.width()/2, y + s.height());
  Vector2<int> bottom(x + s.width()/2, y);
  Vector2<int> left  (x              , y + s.height()/2);
  Vector2<int> right (x + s.width()  , y + s.height()/2);

  unsigned gid = map->getTileGidAt(left.x, left.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(left.x, left.y);
    x = tileRect.right();
  }

  gid = map->getTileGidAt(right.x, right.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(right.x, right.y);
    x = tileRect.left() - s.width();
  }

  gid = map->getTileGidAt(bottom.x, bottom.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(bottom.x, bottom.y);
    y = tileRect.top();
  }

  gid = map->getTileGidAt(top.x, top.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(top.x, top.y);
    y = tileRect.bottom() - s.height();
  }

  s.setPosition(x, y);

  // Center on player
  e->centerOnPixel(s.position().x + s.width()/2, s.position().y + s.height()/2);
}
