#include "Ground.h"

#include "../Box2dPhysicsEngine.h"
#include "../GameLogic.h"

GroundPhysicsComponent::GroundPhysicsComponent(GameLogic* game,
                                               const MapObject& obj)
    : PhysicsComponent()
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(obj.x, obj.y);

    Box2dPhysicsEngine* pe =
        static_cast<Box2dPhysicsEngine*>(game->physicsEngine());
    b2Body* groundBody = pe->world()->CreateBody(&groundBodyDef);

    const std::string& shape = obj.shape;
    if (shape == "polyline" || shape == "polygon") {
        size_t numOfPoints = obj.points.size();
        std::vector<b2Vec2> vs(numOfPoints);

        for (size_t i = 0; i < numOfPoints; ++i) {
            auto& p = obj.points[i];
            vs[i].Set(p.first, p.second);
        }

        b2ChainShape chain;
        if (obj.shape == "polyline")
            ;
        //chain.CreateChain(vs.data(), numOfPoints);
        else
            chain.CreateLoop(vs.data(), numOfPoints);

        groundBody->CreateFixture(&chain, 0.0f);

    } else {
        // Rect object
        b2PolygonShape box;
        float hw = obj.width / 2;
        float hh = obj.height / 2;
        box.SetAsBox(hw, hh, b2Vec2(hw, hh), 0.0f);

        groundBody->CreateFixture(&box, 0.0f);
    }

    b2Filter filter;
    filter.categoryBits = GROUND;
    groundBody->GetFixtureList()->SetFilterData(filter);
    // groundBody->SetUserData(this);

    m_body = groundBody;
}
