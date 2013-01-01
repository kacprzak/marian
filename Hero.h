#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

class Engine;

class Hero : public GameObject
{
 public:
  explicit Hero(Game *game, const Sprite& sprite)
    : GameObject(game, sprite)
    , m_vx(0.0f)
    , m_vy(0.0f)
  {
    resetAcceleration();
  }
  
  void update(Engine *e, float elapsedTime);

 private:
  void resetAcceleration()
  {
    m_ax = 0.0f;
    m_ay = -9.8f * 100.0f;
  }

  float m_ax;
  float m_ay;

  float m_vx;
  float m_vy;
};

#endif
