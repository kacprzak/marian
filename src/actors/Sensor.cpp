/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Sensor.h"

#include "Game.h"
#include <Box2D/Box2D.h>

SensorPhysicsComponent::SensorPhysicsComponent(Game *game, const MapObject &obj)
    : GroundPhysicsComponent(game, obj)
{
    b2Filter filter;
    filter.categoryBits = SENSOR;
    filter.maskBits = HERO;

    // Ground has one fixture
    b2Fixture *fix = m_body->GetFixtureList();
    fix->SetFilterData(filter);
    fix->SetSensor(true);
}
