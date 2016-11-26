/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#include "Actor.h"

#include "Logger.h"

Actor::Actor(ActorId id, GameLogic* game)
    : m_id(id)
    , m_game(game)
    , m_dead(false)
{
    // DBG( "new Actor: id = " << m_id  );
}

//------------------------------------------------------------------------------

Actor::~Actor()
{
    // DBG( "delete Actor: id = " << m_id << " name = " << name() );
}

//------------------------------------------------------------------------------

void Actor::die() { m_dead = true; }
