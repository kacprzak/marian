/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef MAP_H
#define MAP_H

#include "TmxMap.h"

#include <boost/utility.hpp>
#include <string>
#include <vector>

class Tile;
class Layer;

//------------------------------------------------------------------------------

/** Map object but in game coords */
class MapObject
{
 public:
    std::string name;
    std::string type;
    unsigned gid; //< 0 means no gid
    float x;
    float y;
    float width;
    float height;
    bool visible;
    std::string shape; //< ellipse, polygon, polyline or null
    std::vector<std::pair<float, float> > points; //< polygon or polyline points
};

//------------------------------------------------------------------------------

/** Interface to game map in game coords */
class Map : boost::noncopyable
{
    friend class Layer;
    friend class Tile;

 public:
    Map();
    ~Map();
    bool loadFromFile(const std::string& filename);

    int width() const { return m_width; }
    int height() const { return m_height; }

    /** Get map objects */
    void getObjects(std::vector<MapObject>& v);

    std::vector<std::string> externalImages() const;
    std::string backgroundColor() const;

    Layer *findLayer(const std::string& layerName) const;

 private:
    void rectForTile(int tileCoords[4], unsigned globalId) const;

    /** Size in game (tile) coords */
    int m_width;
    int m_height;

    /** Size of one tile in pixels */
    int m_tileWidth;
    int m_tileHeight;

    std::vector<Layer *> m_layers;
    tmx::Map m_tmxMap;
};

//------------------------------------------------------------------------------

class Layer : boost::noncopyable
{
public:
    Layer(const Map *map, const tmx::Layer& layer);
    ~Layer();

    //void draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const;

    const Map *map; //< parent object
    std::string name;
    int width;
    int height;
    bool visible;
    std::vector<Tile *> tiles;
};

//------------------------------------------------------------------------------

class Tile
{
public:
    Tile(const Map *map, unsigned agid);

    unsigned       gid;
    std::string    textureSource;
    float          texCoords[8];
};

#endif
