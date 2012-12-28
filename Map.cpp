#include "Map.h"
#include "Engine.h"
#include "ResourceMgr.h"

// Bits on the far end of the 32-bit global tile ID are used for tile flags
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

Map::Map()
{
  //
}

bool Map::loadFromFile(const std::string& filename)
{
  return m_tmxMap.loadFromFile(filename);
}

void Map::getObjects(std::vector<Sprite>& v)
{
  const Texture *tex = ResourceMgr::instance().getTexture("minecraft_tiles_big.png");

  for (const tmx::ObjectGroup& og : m_tmxMap.objectGroups) {
    for (const tmx::Object& obj : og.objects) {
      Sprite sprite(tex, rectForTile(obj.gid));
      sprite.setPosition(obj.x, m_tmxMap.height * m_tmxMap.tileHeight - obj.y - m_tmxMap.tileHeight);
      v.push_back(sprite);
    }
  }
}

void Map::draw(Engine *e) const
{
  for (const tmx::Layer& layer : m_tmxMap.layers) {
    drawLayer(e, layer);
  }
}

void Map::drawLayer(Engine *e, const tmx::Layer& layer) const
{
  const Texture *tex = ResourceMgr::instance().getTexture("minecraft_tiles_big.png");

  int tileWidth  = m_tmxMap.tileWidth;
  int tileHeight = m_tmxMap.tileHeight;

  for (int y = 0; y < layer.height; ++y) {
    for (int x = 0; x < layer.width; ++x) {

      unsigned global_tile_id = layer.data[y * layer.width + x];

      if (global_tile_id == 0) continue;

      // Read out the flags
      //bool flipped_horizontally = (global_tile_id & FLIPPED_HORIZONTALLY_FLAG);
      //bool flipped_vertically   = (global_tile_id & FLIPPED_VERTICALLY_FLAG);
      //bool flipped_diagonally   = (global_tile_id & FLIPPED_DIAGONALLY_FLAG);

      // Clear the flags
      global_tile_id &= ~(FLIPPED_HORIZONTALLY_FLAG |
                          FLIPPED_VERTICALLY_FLAG |
                          FLIPPED_DIAGONALLY_FLAG);

      Sprite sprite(tex, rectForTile(global_tile_id));

      sprite.setPosition(x * tileWidth, -y * tileHeight + tex->h());
      e->drawSprite(sprite);
    }
  }
}

Rect<int> Map::rectForTile(unsigned global_tile_id) const
{
  for (int i = m_tmxMap.tilesets.size() - 1; i >= 0; --i) {
    const tmx::Tileset& tileset = m_tmxMap.tilesets[i];
    
    if (tileset.firstGid <= global_tile_id) {
      int local_id = global_tile_id - tileset.firstGid;
      
      int width = tileset.imageWidth / tileset.tileWidth;
      //int height = tileset.imageHeight / tileset.tileHeight;

      // Tile coords (y pointing down)
      int local_x = local_id % width;
      int local_y = local_id / width;

      // Pixel coords (y pointing up)
      int opengl_x = local_x * tileset.tileWidth;
      int opengl_y = tileset.imageHeight - (local_y * tileset.tileHeight) - tileset.tileHeight;

      return Rect<int>(opengl_x, opengl_y, tileset.tileWidth, tileset.tileHeight);
    }
  }

  return Rect<int>(0, 0, 0, 0);
}

