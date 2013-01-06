#include "Hero.h"

#include "Engine.h"
#include "Game.h"
#include <iostream>

void Hero::update(Engine *e, float elapsedTime)
{
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

  Sprite& s = m_sprite;
  Map *map = m_game->map();

  Vector2<float> newPos = s.position();

  // Euler integration
  newPos.x += m_vx * elapsedTime;
  newPos.y += m_vy * elapsedTime;

  m_vx += (m_ax * elapsedTime) / 2.0f;
  m_vy += (m_ay * elapsedTime) / 2.0f;


  // Respect map borders and walls
  const Vector2<float>& afterMapCollision = resolveCollisionsWithMap(newPos, map);
  // If there was X collision
  if (newPos.x != afterMapCollision.x) {
    newPos.x = afterMapCollision.x;
  }

  // If there was Y collision
  if (newPos.y != afterMapCollision.y) {
    newPos.y = afterMapCollision.y;
  }

  resetAcceleration();

  // Ladders
  //lb_gid = map->getTileGidAtf(lb.x, lb.y, "ladders");
  //rb_gid = map->getTileGidAtf(rb.x, rb.y, "ladders");
  
  //if (lb_gid || rb_gid) {
    // We are on ladder
    //m_ay = 0.0f;
    //m_vy = 0.0f;
  //}

  s.setPosition(newPos.x, newPos.y);
  // Center on player
  e->centerOnPixel(s.position().x + s.width()/2, s.position().y + s.height()/2);
}

//------------------------------------------------------------------------------

Vector2<float> Hero::resolveCollisionsWithMap(const Vector2<float>& newPos, const Map *map)
{
  Rect<float> newBounds(newPos.x, newPos.y, m_sprite.width(), m_sprite.height());

  // Dont leave the map
  if (newBounds.left() < 0) {
    newBounds.setX(0.0f);
    m_vx = 0.0f;
  }
  if (newBounds.bottom() < 0) {
    newBounds.setY(0.0f);
    m_vy = 0.0f;
  }
  if (newBounds.right() > map->pixelSize().x) {
    newBounds.setX(map->pixelSize().x - newBounds.size().x);
    m_vx = 0.0f;
  }
  if (newBounds.top() > map->pixelSize().y) {
    newBounds.setY(map->pixelSize().y - newBounds.size().y);
    m_vy = 0.0f;
  }

  // Respect walls ;)

  const Vector2<float>& lb = newBounds.leftbottom();
  unsigned lb_gid = map->getTileGidAtf(lb.x, lb.y, "collision");

  if (lb_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(lb.x, lb.y));
    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }


  const Vector2<float>& rb = newBounds.rightbottom();
  unsigned rb_gid = map->getTileGidAtf(rb.x, rb.y, "collision");

  if (rb_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(rb.x, rb.y));
    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }
  
  const Vector2<float>& lt = newBounds.lefttop();
  unsigned lt_gid = map->getTileGidAtf(lt.x, lt.y, "collision");

  if (lt_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(lt.x, lt.y));
    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }
  
  const Vector2<float>& rt = newBounds.righttop();
  unsigned rt_gid = map->getTileGidAtf(rt.x, rt.y, "collision");

  if (rt_gid) {
    Vector2<float> pv = newBounds.escapeVector(map->getTileRectAtf(rt.x, rt.y));
    if (pv.x) m_vx = 0;
    if (pv.y) m_vy = 0;
    newBounds.move(pv);
  }
  
  return newBounds.position();
}
