/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "GameObject.h"
#include "Engine.h"

bool GameObject::processInput(const SDL_Event& /*event*/)
{
    return true;
}

void GameObject::update(Engine * /*e*/, float /*elapsedTime*/)
{
    //
}

void GameObject::draw(Engine *e)
{
    const b2Vec2& pos = m_body->GetPosition();
    e->drawSprite(pos.x, pos.y, m_sprite);
}
