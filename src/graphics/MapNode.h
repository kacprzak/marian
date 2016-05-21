/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef MAPNODE_H
#define MAPNODE_H

#include "Map.h"
#include "HumanView.h"
#include "Renderer.h"

#include <memory>

namespace gfx {

    class MapNode
    {
    public:
        MapNode();

        void setMap(std::shared_ptr<Map> map)
        {
            m_map = map;
            calculateTilesTextureData();
        }

        void update(float elapsedTime);

        int width() const { return m_map->width(); }
        int height() const { return m_map->height(); }

        std::string backgroundColor() const { return m_map->backgroundColor(); }

        void drawBackground(Renderer *rndr, const ViewRect& rect) const;
        void drawForeground(Renderer *rndr, const ViewRect& rect) const;

    private:
        void calculateTilesTextureData();

        //void draw(Engine *e, float xFrom, float xTo, float yFrom, float yTo) const;
        void drawLayer(Renderer *rndr, const std::string& layer, const ViewRect& rect) const;
        void drawParallaxLayer(Renderer *rndr, const std::string& layerName, const ViewRect& rect, float transition) const;

        void drawLayer(Renderer *rndr, const Layer *layer, int xFrom, int xTo, int yFrom, int yTo) const;

        std::shared_ptr<Map> m_map;

        float m_cloudsTransition;
    };

} // namespace gfx

#endif // MAPNODE_H
