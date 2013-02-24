/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <boost/utility.hpp>
#include "TmxMap.h"
#include "Math.h"

class Engine;
class Layer;

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

class Tile;

/** Interface to game map in game coords */
class Map : boost::noncopyable
{
    friend Layer;
    friend Tile;

 public:
    Map();
    ~Map();
    bool loadFromFile(const std::string& filename);

    Vector2<float> size() const;

    void draw(Engine *e, float xFrom, float xTo, float yFrom, float yTo) const;
    void drawLayer(Engine *e, const std::string& layer,
                   float xFrom, float xTo, float yFrom, float yTo) const;

    /** Get map objects */
    void getObjects(std::vector<MapObject>& v);

    std::vector<std::string> externalImages() const;

 private:
    std::string imageForTile(unsigned globalId) const;
    Rect<int> rectForTile(unsigned globalId) const;

    Layer *findLayer(const std::string& layerName) const;

    /** Size in game (tile) coords */
    int m_width;
    int m_height;

    /** Size of one tile in pixels */
    int m_tileWidth;
    int m_tileHeight;

    std::vector<Layer *> m_layers;
    tmx::Map m_tmxMap;
};

#endif
