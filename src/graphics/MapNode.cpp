#include "MapNode.h"
#include "ResourceMgr.h"

#include <cmath>

//#define TEST 1

MapNode::MapNode()
{
}

void MapNode::drawBackground(Engine *e, const ViewRect& r) const
{
    // TODO: read order from map
    drawLayer(e, "back",   r);
    drawLayer(e, "ground", r);
}

void MapNode::drawForeground(Engine *e, const ViewRect& r) const
{
    drawParallaxLayer(e, "water", r);
    drawLayer(e, "front", r);
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

void MapNode::drawLayer(Engine *e, const std::string& layerName, const ViewRect& rect) const
{
    const Layer *layer = m_map->findLayer(layerName);

    if (layer && layer->visible) {
        int x1 = static_cast<int>(std::floor(rect.left));
        int x2 = static_cast<int>(std::ceil(rect.right));
        int y1 = static_cast<int>(std::floor(m_map->height() - rect.top));
        int y2 = static_cast<int>(std::ceil(m_map->height() - rect.bottom));

#if 0
        // For testing
        ++x1; --x2; ++y1; --y2;
#endif

        // Clamp coords
        if (x1 < 0)               x1 = 0;
        if (x2 > m_map->width())  x2 = m_map->width();
        if (y1 < 0)               y1 = 0;
        if (y2 > m_map->height()) y2 = m_map->height();

        drawLayer(e, layer, x1, x2, y1, y2);
    }
}

//------------------------------------------------------------------------------

void MapNode::drawParallaxLayer(Engine *e, const std::string& layerName, const ViewRect &rect) const
{
    const Layer *layer = m_map->findLayer(layerName);

    float parallax = 0.1;

    float parallaxTransition = -rect.left * parallax;

#if TEST
    std::clog << rect.left << " | " << parallaxTransition;
#endif

    ViewRect r = rect;
    //r.left *= parallax;
    //r.right *= parallax;

    if (layer && layer->visible) {
        int x1 = static_cast<int>(std::floor(r.left));
        int x2 = static_cast<int>(std::ceil(r.right));
        int y1 = static_cast<int>(std::floor(m_map->height() - r.top));
        int y2 = static_cast<int>(std::ceil(m_map->height() - r.bottom));

#if 0
        // For testing
        ++x1; --x2; ++y1; --y2;
#endif
        // Clamp coords
        if (x1 < 0)               x1 = 0;
        if (x2 > m_map->width())  x2 = m_map->width();
        if (y1 < 0)               y1 = 0;
        if (y2 > m_map->height()) y2 = m_map->height();

        for (int y = y1; y < y2; ++y) {
            for (int x = x1; x < x2; ++x) {
                const Tile *tile = layer->tiles[y * layer->width + x];

                if (tile) {
                    GLuint textureId = ResourceMgr::singleton().getTexture(tile->textureSource)->textureId();
                    e->drawQuad(static_cast<float>(x) + parallaxTransition,
                                static_cast<float>(m_map->height() - y - 1),
                                1.0f,
                                1.0f,
                                textureId, tile->texCoords);
                }
            }
        }
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
