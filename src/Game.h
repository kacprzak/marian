#ifndef GAME_H
#define GAME_H

#include "Playable.h"
#include <vector>
#include "Map.h"
#include "FpsCounter.h"

class Sprite;
class GameObject;

class Game : public Playable
{
 public:
  Game();
  ~Game();

  // Playable interface
  void initialize(Engine *e);
  void clear(Engine *e);
  bool processInput(const SDL_Event& event);
  void update(Engine *e, float elapsedTime);
  void draw(Engine *e);

  Map *map() { return &m_map; }

 private:
  std::vector<GameObject *> m_gameObjects;
  Map m_map;

  FpsCounter m_fpsCounter;
};

#endif
