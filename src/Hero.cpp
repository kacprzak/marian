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

  //static float v = 250.0f;
  static float frictionFactor = 1.2f;

  if (e->isPressed(SDLK_RIGHT)) {
    if (m_vx < 0.0f)
      m_vx /= frictionFactor;
    m_ax = 1000.0f;
  } else if (e->isPressed(SDLK_LEFT)) {
    if (m_vx > 0.0f)
      m_vx /= frictionFactor;
    m_ax = -1000.0f;
  } else {
    m_vx /= frictionFactor;
  }

  if (e->isPressed(SDLK_UP) && m_vy == 0.0f)   m_vy = 250.0f;
  if (e->isPressed(SDLK_DOWN) && m_vy == 0.0f) m_vy = -250.0f;

  // Euler integration
  x += m_vx * elapsedTime;
  y += m_vy * elapsedTime;

  m_vx += (m_ax * elapsedTime) / 2.0f;
  m_vy += (m_ay * elapsedTime) / 2.0f;

  // Dont leave the map
  if (x < 0) {
    x = 0.0f;
    m_vx = 0.0f;
  }
  if (y < 0) {
    y = 0.0f;
    m_vy = 0.0f;
  }
  if (x > map->pixelSize().x - s.width()) {
    x = map->pixelSize().x - s.width();
    m_vx = 0.0f;
  }
  if (y > map->pixelSize().y - s.height()) {
    y = map->pixelSize().y - s.height();
    m_vy = 0.0f;
  }

  // Respect walls ;)

#if 0
  // Hotspots
  // ..o..
  // .....
  // o...o
  // .....
  // .o.o.

  Vector2<int> top    (x + s.width() * 0.5f , y + s.height());
  Vector2<int> bottom1(x + s.width() * 0.25f, y);
  Vector2<int> bottom2(x + s.width() * 0.75f, y);
  Vector2<int> left   (x                    , y + s.height() / 2);
  Vector2<int> right  (x + s.width()        , y + s.height() / 2);

  // Wall on the left
  unsigned gid = map->getTileGidAt(left.x, left.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(left.x, left.y);
    x = tileRect.right();
    m_vx = 0.0f;
  }

  // Wall on right
  gid = map->getTileGidAt(right.x, right.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(right.x, right.y);
    x = tileRect.left() - s.width();
    m_vx = 0.0f;
  }

  // Ground
  gid = map->getTileGidAt(bottom1.x, bottom1.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(bottom1.x, bottom1.y);
    y = tileRect.top();
    m_vy = 0.0f;
  }

  gid = map->getTileGidAt(bottom2.x, bottom2.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(bottom2.x, bottom2.y);
    y = tileRect.top();
    m_vy = 0.0f;
  }

  // Celling
  gid = map->getTileGidAt(top.x, top.y, "collision");
  if (gid != 0) {
    const Rect<int> tileRect = map->getTileRectAt(top.x, top.y);
    y = tileRect.bottom() - s.height();
    m_vy = 0.0f;
  }

  resetAcceleration();

  bool isOnLadder = false;
  // Ladders
  gid = map->getTileGidAt(bottom1.x, bottom1.y, "ladders");
  if (gid != 0) {
    isOnLadder = true;
  }

  gid = map->getTileGidAt(bottom2.x, bottom2.y, "ladders");
  if (gid != 0) {
    isOnLadder = true;
  }

  if (isOnLadder) {
    m_ay = 0.0f;
    m_vy = 0.0f;
  }

#else
  Rect<float> newBounds(x, y, s.width(), s.height());
  const Vector2<float>& lb = newBounds.leftbottom();
  const Vector2<float>& rb = newBounds.rightbottom();
  const Vector2<float>& lt = newBounds.lefttop();
  const Vector2<float>& rt = newBounds.righttop();

  unsigned lb_gid = map->getTileGidAtf(lb.x, lb.y, "collision");
  unsigned rb_gid = map->getTileGidAtf(rb.x, rb.y, "collision");
  unsigned lt_gid = map->getTileGidAtf(lt.x, lt.y, "collision");
  unsigned rt_gid = map->getTileGidAtf(rt.x, rt.y, "collision");

  if (lb_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(lb.x, lb.y));

    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }

  if (rb_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(rb.x, rb.y));
 
    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }

  if (lt_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(lt.x, lt.y));

    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }

  if (rt_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(rt.x, rt.y));
 
    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }

  x = newBounds.position().x;
  y = newBounds.position().y;

  resetAcceleration();

  // Ladders
  lb_gid = map->getTileGidAtf(lb.x, lb.y, "ladders");
  rb_gid = map->getTileGidAtf(rb.x, rb.y, "ladders");
  if (lb_gid || rb_gid) {
    // We are on ladder
    m_ay = 0.0f;
    m_vy = 0.0f;
  }
#endif


  s.setPosition(x, y);
  // Center on player
  e->centerOnPixel(s.position().x + s.width()/2, s.position().y + s.height()/2);
}
