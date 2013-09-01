#include "MapNode.h"
#include "ResourceMgr.h"

#include <cmath>

//#define TEST 1

MapNode::MapNode()
{
    m_cloudsTransition = 0.0f;
}

//------------------------------------------------------------------------------

void MapNode::update(float elapsedTime)
{
    m_cloudsTransition += 0.2f * elapsedTime;

    if (m_cloudsTransition > width())
        m_cloudsTransition -= width();
}

//------------------------------------------------------------------------------

void MapNode::drawBackground(Engine *e, const ViewRect& r) const
{
    // TODO: read order from map
    drawParallaxLayer(e, "clouds", r, r.left * -0.3f + m_cloudsTransition);
    drawLayer(e, "back",   r);
    drawLayer(e, "ground", r);
}

//------------------------------------------------------------------------------

void MapNode::drawForeground(Engine *e, const ViewRect& r) const
{
    drawParallaxLayer(e, "water", r, r.left * 0.1f);
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

void MapNode::drawParallaxLayer(Engine *e, const std::string& layerName, const ViewRect &rect, float transition) const
{
    const Layer *layer = m_map->findLayer(layerName);

    ViewRect r = rect;
    r.left  += transition;
    r.right += transition;

    if (layer && layer->visible) {
        int x1 = static_cast<int>(std::floor(r.left));
        int x2 = static_cast<int>(std::ceil(r.right));
        int y1 = static_cast<int>(std::floor(m_map->height() - r.top));
        int y2 = static_cast<int>(std::ceil(m_map->height() - r.bottom));

#if 0
        // For testing
        ++x1; --x2; ++y1; --y2;
#endif
        for (int y = y1; y < y2; ++y) {
            for (int x = x1; x < x2; ++x) {               
                int tile_x = x % layer->width;
                int tile_y = y % layer->height;
                while (tile_x < 0) tile_x += layer->width;
                while (tile_y < 0) tile_y += layer->height;

                const Tile *tile = layer->tiles[tile_y * layer->width + tile_x];

                if (tile) {
                    GLuint textureId = ResourceMgr::singleton().getTexture(tile->textureSource)->textureId();
                    e->drawQuad(static_cast<float>(x) - transition,
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
