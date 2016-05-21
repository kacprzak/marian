/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "SpriteNode.h"

using namespace gfx;

SpriteNode::SpriteNode()
    : m_actorId(0)
    , m_image(nullptr)
    , m_x(0.0f)
    , m_y(0.0f)
    , m_rot(0.0f)
    , m_flippedVert(false)
    , m_flippedHori(false)
{
}

//------------------------------------------------------------------------------

void SpriteNode::setImage(const Image& image)
{
    m_image = new Image(image);
}

//------------------------------------------------------------------------------

void SpriteNode::moveTo(float x, float y, float angle)
{
    m_x = x;
    m_y = y;
    m_rot = angle;
}

//------------------------------------------------------------------------------

void SpriteNode::flipHorizontally(bool flip)
{
    m_flippedHori = flip;
}

//------------------------------------------------------------------------------

void SpriteNode::flipVertically(bool flip)
{
    m_flippedVert = flip;
}
