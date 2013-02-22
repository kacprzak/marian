/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Hero.h"

#include "Engine.h"
#include "Game.h"
#include <iostream>

#define GRAVITY -9.8f * 100.0f
#define BOUNCE_EPSILON 150.0f
#define BOUNCE_FACTOR 0.5f

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

    if (e->isPressed(SDLK_UP) && m_vy == 0.0f)   m_vy = 400.0f;
    if (e->isPressed(SDLK_DOWN) && m_vy == 0.0f) m_vy = -250.0f;

    Sprite& s = m_sprite;
    Map *map = m_game->map();

    Vector2<float> newPos = s.position();

    // Euler integration
    newPos.x += m_vx * elapsedTime;
    newPos.y += m_vy * elapsedTime;

    m_vx += m_ax * elapsedTime;
    m_vy += m_ay * elapsedTime;

    // Reset acceleration
    m_ax = 0.0f;
    m_ay = GRAVITY;

    // Respect map borders and walls
    const Vector2<float>& afterMapCollision = resolveCollisionsWithMap(newPos, map);

    // If there was X collision
    if (newPos.x != afterMapCollision.x) {
        newPos.x = afterMapCollision.x;
        if (std::abs(m_vx) < BOUNCE_EPSILON)
            m_vx = 0.0f;
        else
            m_vx = -m_vx * BOUNCE_FACTOR;
    }

    // If there was Y collision
    if (newPos.y != afterMapCollision.y) {
        newPos.y = afterMapCollision.y;
        // Bounce only on top collision
        if (m_vy < BOUNCE_EPSILON) 
            m_vy = 0.0f;
        else
            m_vy = -m_vy * BOUNCE_FACTOR;
    }

    s.setPosition(newPos.x, newPos.y);

    // Ladders
    unsigned lb_gid = map->getTileGidAtf(s.position().x, s.position().y, "ladders");
    unsigned rb_gid = map->getTileGidAtf(s.position().x + s.width(), s.position().y, "ladders");
  
    if (lb_gid || rb_gid) {
        // We are on ladder
        m_ay = 0.0f;
        m_vy = 0.0f;
    }

    // Center view on player
    e->centerViewOn(s.position().x + s.width()/2, s.position().y + s.height()/2);
}

//------------------------------------------------------------------------------

Vector2<float> Hero::resolveCollisionsWithMap(const Vector2<float>& newPos, const Map *map) const
{
    Rect<float> newBounds(newPos.x, newPos.y, m_sprite.width(), m_sprite.height());

    // Dont leave the map
    if (newBounds.left() < 0)
        newBounds.setX(0.0f);
  
    if (newBounds.bottom() < 0)
        newBounds.setY(0.0f);
  
    if (newBounds.right() > map->pixelSize().x)
        newBounds.setX(map->pixelSize().x - newBounds.size().x);
  
    if (newBounds.top() > map->pixelSize().y)
        newBounds.setY(map->pixelSize().y - newBounds.size().y);
  
    // Respect walls ;)
  
    const Vector2<float>& lb = newBounds.leftbottom();
    unsigned lb_gid = map->getTileGidAtf(lb.x, lb.y, "collision");
  
    if (lb_gid) {
        const Vector2<float>& pv = newBounds.escapeVector(map->getTileRectAtf(lb.x, lb.y));
        newBounds.move(pv);
    }
  
    const Vector2<float>& rb = newBounds.rightbottom();
    unsigned rb_gid = map->getTileGidAtf(rb.x, rb.y, "collision");
  
    if (rb_gid) {
        const Vector2<float>& pv = newBounds.escapeVector(map->getTileRectAtf(rb.x, rb.y));
        newBounds.move(pv);
    }
  
    const Vector2<float>& lt = newBounds.lefttop();
    unsigned lt_gid = map->getTileGidAtf(lt.x, lt.y, "collision");
  
    if (lt_gid) {
        const Vector2<float>& pv = newBounds.escapeVector(map->getTileRectAtf(lt.x, lt.y));
        newBounds.move(pv);
    }
  
    const Vector2<float>& rt = newBounds.righttop();
    unsigned rt_gid = map->getTileGidAtf(rt.x, rt.y, "collision");
  
    if (rt_gid) {
        const Vector2<float>& pv = newBounds.escapeVector(map->getTileRectAtf(rt.x, rt.y));
        newBounds.move(pv);
    }
  
    return newBounds.position();
}
