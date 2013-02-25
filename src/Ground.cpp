/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Ground.h"

#include "Game.h"
#include <Box2D/Box2D.h>

Ground::Ground(Game *game, const MapObject& obj)
    : GameObject(game, nullptr)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(obj.x, obj.y);

    b2Body *groundBody = m_game->world()->CreateBody(&groundBodyDef);

    const std::string& shape = obj.shape;
    if (shape == "polyline" || shape == "polygon") {
        size_t numOfPoints = obj.points.size();
        b2Vec2 vs[numOfPoints];
                
        for (size_t i = 0; i < numOfPoints; ++i) {
            auto& p = obj.points[i];
            vs[i].Set(p.first, p.second);
        }

        b2ChainShape chain;
        if (obj.shape == "polyline")
            chain.CreateChain(vs, numOfPoints);
        else
            chain.CreateLoop(vs, numOfPoints);

        groundBody->CreateFixture(&chain, 0.0f);

    } else {
        // Rect object
        b2PolygonShape box;
        float hw = obj.width / 2;
        float hh = obj.height / 2;
        box.SetAsBox(hw, hh, b2Vec2(hw, hh), 0.0f);

        groundBody->CreateFixture(&box, 0.0f);
    }

    m_body = groundBody;
}
