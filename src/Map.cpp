/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Map.h"
#include "Engine.h"
#include "ResourceMgr.h"
#include "Util.h"

#include <cstring>
#include <iostream>

class Tile
{
public:
    Tile(const Map *map, unsigned agid, const Texture *tex)
        : gid(agid)
        , texture(tex)
    {
        calculateTextureCoords(tex->w(), tex->h(),
                               map->rectForTile(gid),
                               texCoords);
    }
  
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

Layer::~Layer()
{
    for (const Tile* tile : tiles) {
        delete tile;
    }
}

void Layer::draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const
{
    for (int y = yFrom; y < yTo; ++y) {
        for (int x = xFrom; x < xTo; ++x) {
            const Tile *tile = tiles[y * width + x];
            if (tile) {
                e->drawQuad(x,
                            map->m_height - y - 1.0f,
                            1.0f,
                            1.0f,
                            tile->texture->textureId(), tile->texCoords);
            }
        }
    }
}

//##############################################################################

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

                mapObject.points.push_back({x, y});
            }

            v.push_back(mapObject);
        }
    }
}

//------------------------------------------------------------------------------

void Map::draw(Engine *e, float xFrom, float xTo, float yFrom, float yTo) const
{
    // No bounds checking // Fix me

    for (const Layer* layer : m_layers) {
        layer->draw(e, xFrom, xTo, yFrom, yTo);
    }
}

//------------------------------------------------------------------------------

void Map::drawLayer(Engine *e, const std::string& layerName,
                    float xFrom, float xTo, float yFrom, float yTo) const
{
    const Layer *layer = findLayer(layerName);

    if (layer && layer->visible) {
        xFrom = std::floor(xFrom);
        xTo   = std::ceil(xTo);
        float tmp = yFrom;
        yFrom = std::floor(m_height - yTo);
        yTo   = std::ceil(m_height - tmp);
        
        // Clamp coords
        if (xFrom < 0) xFrom = 0;
        if (xTo   > m_width) xTo = m_width;
        if (yFrom < 0) yFrom = 0;
        if (yTo   > m_height) yTo = m_height;
        
        layer->draw(e, xFrom, xTo, yFrom, yTo);
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

//------------------------------------------------------------------------------

Image Map::imageForTile(unsigned gid) const
{
    std::string imageSource = imageNameForTile(gid);
    const Texture *tex = ResourceMgr::instance().getTexture(imageSource);

    return Image(tex, rectForTile(gid));
}
