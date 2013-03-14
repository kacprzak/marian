/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Map.h"
#include "Engine.h"
#include "ResourceMgr.h"

#include <cstring>
#include <iostream>
#include <cmath>

class Tile
{
public:
    Tile(const Map *map, unsigned agid, const Texture *tex)
        : gid(agid)
        , texture(tex)
    {
        int tileCoords[4];
        map->rectForTile(tileCoords, gid);
        Texture::calculateTextureCoords(texCoords, tex->w(), tex->h(),
                                        tileCoords[0], tileCoords[1],
                                        tileCoords[2], tileCoords[3]);
    }
  
    unsigned       gid;
    const Texture *texture;
    GLfloat        texCoords[8];
};

//==============================================================================

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
    bool visible;
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
    , visible((tmxLayer.visible == "1"))
    , tiles(width * height, nullptr)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
      
            unsigned global_tile_id = tmxLayer.data[y * tmxLayer.width + x];
      
            if (global_tile_id != 0) {
                std::string imageSource = map->imageNameForTile(global_tile_id);
                const Texture *tex = ResourceMgr::instance().getTexture(imageSource);

                // Read out the flags
                //bool flipped_horizontally = (global_tile_id & FLIPPED_HORIZONTALLY_FLAG);
                //bool flipped_vertically   = (global_tile_id & FLIPPED_VERTICALLY_FLAG);
                //bool flipped_diagonally   = (global_tile_id & FLIPPED_DIAGONALLY_FLAG);
        
                // Clear the flags
                global_tile_id &= ~(FLIPPED_HORIZONTALLY_FLAG
                                    | FLIPPED_VERTICALLY_FLAG
                                    | FLIPPED_DIAGONALLY_FLAG);
        
                Tile *tile = new Tile(map, global_tile_id, tex);
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

//------------------------------------------------------------------------------

void Layer::draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const
{
    for (int y = yFrom; y < yTo; ++y) {
        for (int x = xFrom; x < xTo; ++x) {
            const Tile *tile = tiles[y * width + x];
            if (tile) {
                e->drawQuad(static_cast<float>(x),
                            static_cast<float>(map->m_height - y - 1),
                            1.0f,
                            1.0f,
                            tile->texture->textureId(), tile->texCoords);
            }
        }
    }
}

//==============================================================================

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

void Map::draw(Engine *e, float xFrom, float xTo, float yFrom, float yTo) const
{
    // No bounds checking // Fix me

    //for (const Layer* layer : m_layers) {
    //    layer->draw(e, xFrom, xTo, yFrom, yTo);
    //}
}

//------------------------------------------------------------------------------

void Map::drawLayer(Engine *e, const std::string& layerName,
                    float xFrom, float xTo, float yFrom, float yTo) const
{
    const Layer *layer = findLayer(layerName);

    if (layer && layer->visible) {
        int x1 = static_cast<int>(std::floor(xFrom));
        int x2 = static_cast<int>(std::ceil(xTo));
        int y1 = static_cast<int>(std::floor(m_height - yTo));
        int y2 = static_cast<int>(std::ceil(m_height - yFrom));
        
        // Clamp coords
        if (x1 < 0)        x1 = 0;
        if (x2 > m_width)  x2 = m_width;
        if (y1 < 0)        y1 = 0;
        if (y2 > m_height) y2 = m_height;
        
        layer->draw(e, x1, x2, y1, y2);
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

std::string Map::backgroundColor() const
{
    return m_tmxMap.backgroundColor;
}

//------------------------------------------------------------------------------

std::string Map::imageNameForTile(unsigned global_tile_id) const
{
    return m_tmxMap.tilesetForTile(global_tile_id)->imageSource;
}

//------------------------------------------------------------------------------
/**
 * Pixel coords rectangle that bounds image on it's texture.
 */
void Map::rectForTile(int tileCoords[4], unsigned global_tile_id) const
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
  
    tileCoords[0] = opengl_x;
    tileCoords[1] = opengl_y;
    tileCoords[2] = opengl_x + tileset->tileWidth;
    tileCoords[3] = opengl_y + tileset->tileHeight;
}

//------------------------------------------------------------------------------

