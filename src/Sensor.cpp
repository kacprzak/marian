/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Sensor.h"

#include "Game.h"
#include <Box2D/Box2D.h>

Sensor::Sensor(unsigned long id, Game *game, const MapObject& obj)
    : Ground(id, game, obj)
{
    // Ground has one fixture
    b2Fixture *fix = m_body->GetFixtureList();
    fix->SetSensor(true);

    b2Filter filter;
    filter.categoryBits = category();
    filter.maskBits = HERO;
    fix->SetFilterData(filter);
}
