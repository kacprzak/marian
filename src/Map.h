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

/** Map object but in game pixel coords */
class MapObject
{
 public:
    std::string name;
    std::string type;
    unsigned gid; //< 0 is no gid
    int x;
    int y;
    int width;
    int height;
    bool visible;
    std::string shape;   //< ellipse, polygon, polyline or null
    std::vector<std::pair<int, int> > points; //< polygon or polyline points
};

//------------------------------------------------------------------------------

class Map : boost::noncopyable
{
 public:
    Map();
    ~Map();
    bool loadFromFile(const std::string& filename);

    Vector2<int> pixelSize() const;
    Vector2<int> tileSize() const;

    int tileWidth() const { return m_tmxMap.tileWidth; }
    int tileHeight() const { return m_tmxMap.tileHeight; }

    void draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const;
    void drawLayer(Engine *e, const std::string& layer,
                   int xFrom, int xTo, int yFrom, int yTo) const;

    /** Get map objects */
    void getObjects(std::vector<MapObject>& v);

    unsigned getTileGidAt(int x, int y, const std::string& layer) const;
    unsigned getTileGidAtf(float x, float y, const std::string& layer) const
    {
        return getTileGidAt(std::round(x), std::round(y), layer);
    }

    Rect<int> getTileRectAt(int x, int y) const;
    Rect<float> getTileRectAtf(float x, float y) const
        {
            const Rect<int>& rect = getTileRectAt(std::round(x), std::round(y));

            return Rect<float>(rect.position().x,
                               rect.position().y,
                               rect.size().x,
                               rect.size().y); 
        }

    std::vector<std::string> externalImages() const;

    std::string imageForTile(unsigned globalId) const;
    Rect<int> rectForTile(unsigned globalId) const;

 private:
    tmx::Map m_tmxMap;  
    std::vector<Layer *> m_layers;
};

#endif
