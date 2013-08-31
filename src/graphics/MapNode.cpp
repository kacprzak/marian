#include "MapNode.h"
#include "ResourceMgr.h"

#include <cmath>

MapNode::MapNode()
{
}

//------------------------------------------------------------------------------

#if 0
void Map::draw(Engine *e, float xFrom, float xTo, float yFrom, float yTo) const
{
    // No bounds checking // Fix me

    for (const Layer* layer : m_layers) {
        layer->draw(e, xFrom, xTo, yFrom, yTo);
    }
}
#endif

//------------------------------------------------------------------------------

void MapNode::drawLayer(Engine *e, const std::string& layerName,
                        float xFrom, float xTo, float yFrom, float yTo) const
{
    const Layer *layer = m_map->findLayer(layerName);

    if (layer && layer->visible) {
        int x1 = static_cast<int>(std::floor(xFrom));
        int x2 = static_cast<int>(std::ceil(xTo));
        int y1 = static_cast<int>(std::floor(m_map->height() - yTo));
        int y2 = static_cast<int>(std::ceil(m_map->width() - yFrom));

        // Clamp coords
        if (x1 < 0)               x1 = 0;
        if (x2 > m_map->width())  x2 = m_map->width();
        if (y1 < 0)               y1 = 0;
        if (y2 > m_map->height()) y2 = m_map->height();

        drawLayer(e, layer, x1, x2, y1, y2);
    }
}

//------------------------------------------------------------------------------

void MapNode::drawLayer(Engine *e, const Layer *layer, int xFrom, int xTo, int yFrom, int yTo) const
{
    for (int y = yFrom; y < yTo; ++y) {
        for (int x = xFrom; x < xTo; ++x) {
            const Tile *tile = layer->tiles[y * layer->width + x];

            if (tile) {
                GLuint textureId = ResourceMgr::singleton().getTexture(tile->textureSource)->textureId();
                e->drawQuad(static_cast<float>(x),
                            static_cast<float>(m_map->height() - y - 1),
                            1.0f,
                            1.0f,
                            textureId, tile->texCoords);
            }
        }
    }
}
