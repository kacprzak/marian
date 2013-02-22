/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

class Engine;
class Map;

class Hero : public GameObject
{
 public:
    explicit Hero(Game *game, const Sprite& sprite, b2Body *body)
        : GameObject(game, sprite, body)
        , m_vx(0.0f)
        , m_vy(0.0f)
        , m_ax(0.0f)
        , m_ay(0.0f)
        {
            //
        }
  
    void update(Engine *e, float elapsedTime);

 private:
    Vector2<float> resolveCollisionsWithMap(const Vector2<float>& newPos,
                                            const Map *map) const;

    float m_vx;
    float m_vy;

    float m_ax;
    float m_ay;
};

#endif
