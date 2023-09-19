#include "Sensor.h"

#include "../GameLogic.h"

#include <Box2D/Box2D.h>

SensorPhysicsComponent::SensorPhysicsComponent(GameLogic* game,
                                               const MapObject& obj)
    : GroundPhysicsComponent(game, obj)
{
    b2Filter filter;
    filter.categoryBits = SENSOR;
    filter.maskBits     = HERO;

    // Ground has one fixture
    b2Fixture* fix = m_body->GetFixtureList();
    fix->SetFilterData(filter);
    fix->SetSensor(true);
}
