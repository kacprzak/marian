#include "Map.h"
#include "Engine.h"
#include "ResourceMgr.h"

#include <cstring>
#include <iostream>

#include "Util.h"

class Tile
{
 public:
  Tile()
    : gid(0)
    , texture(nullptr)
  {}
  
  unsigned       gid;
  const Texture *texture;
  GLfloat        texCoords[8];
};

class Layer : boost::noncopyable
{
 public:
  Layer(const Map *map, const tmx::Layer& layer);
  ~Layer();

  void draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const;  

  const Map *map; //< parent object
  std::string name;
  int width;
  int height;
  std::vector<Tile *> tiles;
};

//------------------------------------------------------------------------------

// Bits on the far end of the 32-bit global tile ID are used for tile flags
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

Layer::Layer(const Map *parent, const tmx::Layer& tmxLayer)
  : map(parent)
  , name(tmxLayer.name)
  , width(tmxLayer.width)
  , height(tmxLayer.height)
  , tiles(width * height, nullptr)
{
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      
      unsigned global_tile_id = tmxLayer.data[y * tmxLayer.width + x];
      
      if (global_tile_id != 0) {
        std::string imageSource = map->imageForTile(global_tile_id);
        const Texture *tex = ResourceMgr::instance().getTexture(imageSource);

        // Read out the flags
        //bool flipped_horizontally = (global_tile_id & FLIPPED_HORIZONTALLY_FLAG);
        //bool flipped_vertically   = (global_tile_id & FLIPPED_VERTICALLY_FLAG);
        //bool flipped_diagonally   = (global_tile_id & FLIPPED_DIAGONALLY_FLAG);
        
        // Clear the flags
        global_tile_id &= ~(FLIPPED_HORIZONTALLY_FLAG
                            | FLIPPED_VERTICALLY_FLAG
                            | FLIPPED_DIAGONALLY_FLAG);
        
        Tile *tile = new Tile;
        tile->gid = global_tile_id;
        tile->texture = tex;
        calculateTextureCoords(tex->w(), tex->h(), map->rectForTile(global_tile_id),
                               tile->texCoords);
        tiles[y * width + x] = tile;
      }
    }
  }
}

Layer::~Layer()
{
  for (const Tile* tile : tiles) {
    delete tile;
  }
}

void Layer::draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const
{
  int tileWidth  = map->tileWidth();
  int tileHeight = map->tileHeight();

  for (int y = yFrom; y < yTo; ++y) {
    for (int x = xFrom; x < xTo; ++x) {
      const Tile *tile = tiles[y * width + x];
      if (tile) {
        e->drawQuad(x * tileWidth, (map->tileSize().y - y - 1) * tileHeight,
                    tileWidth, tileHeight,
                    tile->texture->textureId(), tile->texCoords);
      }
    }
  }
}

//##############################################################################

Map::Map()
{
  //
}

//------------------------------------------------------------------------------

Map::~Map()
{
  for (const Layer* layer : m_layers) {
    delete layer;
  }
}

//------------------------------------------------------------------------------

bool Map::loadFromFile(const std::string& filename)
{
  m_tmxMap.loadFromFile(filename);

  auto images = externalImages();
  for (const std::string& image : images) {
    if (!ResourceMgr::instance().addTexture(image)) {
      std::cerr << "Unable to load texture.\n";
    }     
  }

  for (const tmx::Layer& layer : m_tmxMap.layers) {
    m_layers.push_back(new Layer(this, layer));
  }
  return true;
}

//------------------------------------------------------------------------------

/**
 * Size of map in pixels.
 */
Vector2<int> Map::pixelSize() const
{
  int w = m_tmxMap.width  * m_tmxMap.tileWidth;
  int h = m_tmxMap.height * m_tmxMap.tileHeight;

  return Vector2<int>(w, h);
}

//------------------------------------------------------------------------------

/**
 * Size of map in tiles.
 */
Vector2<int> Map::tileSize() const
{
  return Vector2<int>(m_tmxMap.width, m_tmxMap.height);
}

//------------------------------------------------------------------------------

void Map::getObjects(std::vector<MapObject>& v)
{
  for (const tmx::ObjectGroup& og : m_tmxMap.objectGroups) {
    for (const tmx::Object& obj : og.objects) {

      MapObject mapObject;
      mapObject.name = obj.name;
      mapObject.type = obj.type;
      mapObject.gid = obj.gid;
      mapObject.x = obj.x;
      mapObject.y = m_tmxMap.height * m_tmxMap.tileHeight - obj.y;
      
      mapObject.width = obj.width;
      mapObject.height = obj.height;

      mapObject.shape = obj.shape;
      mapObject.visible = (obj.visible == "1") ? true : false;

      for (const std::pair<int, int>& point : obj.points) {
        int x = mapObject.x + point.first;
        int y = mapObject.y - point.second;

        mapObject.points.push_back({x, y});
      }

      v.push_back(mapObject);
    }
  }
}

//------------------------------------------------------------------------------

/**
 * GlobalID of tile in position (x,y) i selected layer.
 *
 * @param x pixel coord
 * @param y pixel coord (y points up)
 */
unsigned Map::getTileGidAt(int x, int y, const std::string& layerName) const
{
  for (const tmx::Layer& layer : m_tmxMap.layers) {
    if (layer.name == layerName) {
      // Map coords (y points down)
      int tile_x = x / m_tmxMap.tileWidth;
      int tile_y = m_tmxMap.height - (y / m_tmxMap.tileHeight) - 1;

      return layer.data[tile_y * layer.width + tile_x];
    }
  }

  return 0;
}

//------------------------------------------------------------------------------

Rect<int> Map::getTileRectAt(int x, int y) const
{
  int t_w = m_tmxMap.tileWidth;
  int t_h = m_tmxMap.tileHeight;

  // Purposely int/int
  int t_x = x / t_w;
  int t_y = y / t_h;

  return Rect<int>(t_x * t_w, t_y * t_h, t_w, t_h);
}

//------------------------------------------------------------------------------

void Map::draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const
{
  for (const Layer* layer : m_layers) {
    layer->draw(e, xFrom, xTo, yFrom, yTo);
  }
}

//------------------------------------------------------------------------------

void Map::drawLayer(Engine *e, const std::string& layerName,
                    int xFrom, int xTo, int yFrom, int yTo) const
{
  for (const Layer* layer : m_layers) {
    if (layer->name == layerName)
      layer->draw(e, xFrom, xTo, yFrom, yTo);
  }
}

//------------------------------------------------------------------------------

/**
 * Names of images used in this map.
 */
std::vector<std::string> Map::externalImages() const
{
  std::vector<std::string> ret;

  for (const tmx::Tileset& tileset : m_tmxMap.tilesets) {
    ret.push_back(tileset.imageSource);
  }

  return ret;
}

//------------------------------------------------------------------------------

std::string Map::imageForTile(unsigned global_tile_id) const
{
  return m_tmxMap.tilesetForTile(global_tile_id)->imageSource;
}

//------------------------------------------------------------------------------

/**
 * Pixel coords rectangle that bounds image on it's texture.
 */
Rect<int> Map::rectForTile(unsigned global_tile_id) const
{
  const tmx::Tileset *tileset = m_tmxMap.tilesetForTile(global_tile_id);
  
  int local_id = global_tile_id - tileset->firstGid;
  
  int width = tileset->imageWidth / tileset->tileWidth;
  //int height = tileset->imageHeight / tileset->tileHeight;
  
  // Tile coords (y pointing down)
  int local_x = local_id % width;
  int local_y = local_id / width;
  
  // Pixel coords (y pointing up)
  int opengl_x = local_x * tileset->tileWidth;
  int opengl_y = tileset->imageHeight - (local_y * tileset->tileHeight) - tileset->tileHeight;
  
  return Rect<int>(opengl_x, opengl_y, tileset->tileWidth, tileset->tileHeight);
}
