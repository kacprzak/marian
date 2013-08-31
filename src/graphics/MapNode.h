#ifndef MAPNODE_H
#define MAPNODE_H

#include "Map.h"
#include <memory>
#include "Engine.h"

class MapNode
{
public:
    MapNode();

    void setMap(std::shared_ptr<Map> map) { m_map = map; }

    int width() const { return m_map->width(); }
    int height() const { return m_map->height(); }

    std::string backgroundColor() const { return m_map->backgroundColor(); }

    void drawBackground(Engine *e, const ViewRect& rect) const;
    void drawForeground(Engine *e, const ViewRect& rect) const;

private:
    //void draw(Engine *e, float xFrom, float xTo, float yFrom, float yTo) const;
    void drawLayer(Engine *e, const std::string& layer, const ViewRect& rect) const;
    void drawParallaxLayer(Engine *e, const std::string& layerName, const ViewRect& rect) const;

    void drawLayer(Engine *e, const Layer *layer, int xFrom, int xTo, int yFrom, int yTo) const;

    std::shared_ptr<Map> m_map;
};

#endif // MAPNODE_H
