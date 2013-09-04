/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include "Actor.h"

#include "Logger.h"

Actor::Actor(ActorId id, GameLogic *game)
    : m_id(id)
    , m_game(game)
    , m_dead(false)
{
    DOUT( LOG << "new Actor: id = " << m_id << '\n' );
}

//------------------------------------------------------------------------------

Actor::~Actor() {
    DOUT( LOG << "delete Actor: id = " << m_id << " name = " << name() << '\n' );
}

//------------------------------------------------------------------------------

void Actor::die() {
    //DOUT( LOG << "Actor died: id = " << m_id << " name = " << name() << '\n' );

    m_dead = true;
}
