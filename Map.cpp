#include "Map.h"
#include "Engine.h"
#include "ResourceMgr.h"

Map::Map()
{
  //
}

bool Map::loadFromFile(const std::string& filename)
{
  return m_tmxMap.loadFromFile(filename);
}

void Map::draw(Engine *e)
{
  const Texture *tex = ResourceMgr::instance().getTexture("minecraft_tiles_big.png");

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 16; ++j) {
      int tilesize = 32 * 1;
      int x = i * tilesize;
      int y = -j * tilesize + tex->h() - tilesize;
      
      Sprite sprite(tex, Rect<int>(x, y, tilesize, tilesize));
      sprite.setPosition(x, y);
      e->drawSprite(sprite);
    }
  }
}
