#ifndef GAME_H
#define GAME_H

#include "Playable.h"
#include <vector>
#include "Map.h"

class Sprite;

class Game : public Playable
{
 public:
  Game();
  ~Game();

  // Playable interface
  bool processInput(const SDL_Event& event);
  void update(Engine *e, float elapsedTime);
  void draw(Engine *e);

 private:
  std::vector<Sprite> m_sprites;
  Map m_map;
  bool m_keys[SDLK_LAST];
};

#endif
