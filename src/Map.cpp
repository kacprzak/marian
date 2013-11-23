/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Map.h"

Map::Map()
    : m_width(0)
    , m_height(0)
    , m_tileWidth(32)
    , m_tileHeight(32)
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

    m_width = m_tmxMap.width;
    m_height = m_tmxMap.height;

    m_tileWidth = m_tmxMap.tileWidth;
    m_tileHeight = m_tmxMap.tileHeight;

    for (const tmx::Layer& layer : m_tmxMap.layers) {
        m_layers.push_back(new Layer(this, layer));
    }
    return true;
}

//------------------------------------------------------------------------------

void Map::getObjects(std::vector<MapObject>& v)
{
    for (const tmx::ObjectGroup& og : m_tmxMap.objectGroups) {
        for (const tmx::Object& obj : og.objects) {

            MapObject mapObject;
            mapObject.name = obj.name;
            mapObject.type = obj.type;
            mapObject.gid  = obj.gid;

            mapObject.x = obj.x / float(m_tileWidth);
            mapObject.y = m_height - (obj.height + obj.y) / float(m_tileHeight);
      
            mapObject.width   = obj.width  / float(m_tileWidth);
            mapObject.height  = obj.height / float(m_tileHeight);

            mapObject.shape   = obj.shape;
            mapObject.visible = (obj.visible == "1") ? true : false;

            for (const std::pair<int, int>& point : obj.points) {
                float x = point.first / float(m_tileWidth);
                float y = -point.second / float(m_tileHeight);

                mapObject.points.push_back(std::pair<float, float>(x, y));
            }

            v.push_back(mapObject);
        }
    }
}

//------------------------------------------------------------------------------

Layer * Map::findLayer(const std::string& layerName) const
{
    for (Layer* layer : m_layers) {
        if (layer->name == layerName)
            return layer;
    }
    return nullptr;
}

//------------------------------------------------------------------------------

/*!
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

std::string Map::backgroundColor() const
{
    return m_tmxMap.backgroundColor;
}

//------------------------------------------------------------------------------
/*!
 * Pixel coords rectangle that bounds image on it's texture.
 *
 * @param tileCoords    output argument
 */
void Map::rectOnTextureForTile(Rect<int> *tileCoords, unsigned global_tile_id) const
{
    const tmx::Tileset *tileset = m_tmxMap.tilesetForTile(global_tile_id);
  
    int local_id = global_tile_id - tileset->firstGid;
  
    int width = tileset->imageWidth / tileset->tileWidth;
    //int height = tileset->imageHeight / tileset->tileHeight;
  
    // Tile coords (y pointing down)
    int local_x = local_id % width;
    int local_y = local_id / width;
  
    int margin = tileset->margin;
    int spacing = tileset->spacing;

    // Pixel coords (y pointing up)
    int opengl_x = local_x * tileset->tileWidth + local_x * spacing + margin;
    int opengl_y = tileset->imageHeight - (local_y * tileset->tileHeight) - tileset->tileHeight - local_y * spacing - margin;
  
    tileCoords->left   = opengl_x;
    tileCoords->bottom = opengl_y;
    tileCoords->right  = opengl_x + tileset->tileWidth;
    tileCoords->top    = opengl_y + tileset->tileHeight;
}

//==============================================================================

// Bits on the far end of the 32-bit global tile ID are used for tile flags
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

Layer::Layer(const Map *aMap, const tmx::Layer& tmxLayer)
    : map(aMap)
    , name(tmxLayer.name)
    , width(tmxLayer.width)
    , height(tmxLayer.height)
    , visible((tmxLayer.visible == "1"))
    , tiles(width * height, nullptr)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            unsigned global_tile_id = tmxLayer.data[y * tmxLayer.width + x];

            if (global_tile_id != 0) {
                // Read out the flags
                //bool flipped_horizontally = (global_tile_id & FLIPPED_HORIZONTALLY_FLAG);
                //bool flipped_vertically   = (global_tile_id & FLIPPED_VERTICALLY_FLAG);
                //bool flipped_diagonally   = (global_tile_id & FLIPPED_DIAGONALLY_FLAG);

                // Clear the flags
                global_tile_id &= ~(FLIPPED_HORIZONTALLY_FLAG
                                    | FLIPPED_VERTICALLY_FLAG
                                    | FLIPPED_DIAGONALLY_FLAG);

                Tile *tile = new Tile(map, global_tile_id);
                tiles[y * width + x] = tile;
            }
        }
    }
}

//------------------------------------------------------------------------------

Layer::~Layer()
{
    for (const Tile* tile : tiles) {
        delete tile;
    }
}

//==============================================================================

Tile::Tile(const Map *aMap, unsigned agid)
    : map(aMap)
    , gid(agid)
    , texId(0)
    , texCoords()
{
}

//------------------------------------------------------------------------------

std::string Tile::textureSource() const
{
    const tmx::Tileset *tileset = map->m_tmxMap.tilesetForTile(gid);
    return tileset->imageSource;
}

//------------------------------------------------------------------------------

Rect<int> Tile::tileCoords() const
{
    Rect<int> aTileCoords;
    map->rectOnTextureForTile(&aTileCoords, gid);

    return aTileCoords;
}


