#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

class Engine;

class Hero : public GameObject
{
 public:
  explicit Hero(Game *game, const Sprite& sprite)
    : GameObject(game, sprite)
    {}
  
  void update(Engine *e, float elapsedTime);

};

#endif
