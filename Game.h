#ifndef GAME_H
#define GAME_H

#include "Playable.h"
#include <vector>

class Sprite;

class Game : public Playable
{
 public:
  Game();
  ~Game();

  // Playable interface
  void processInput(const SDL_Event& event);
  void update(Engine *e, float elapsedTime);
  void draw(Engine *e);

 private:
  std::vector<Sprite *> m_sprites;
};

#endif
