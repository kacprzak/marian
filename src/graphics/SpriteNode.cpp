#include "SpriteNode.h"

SpriteNode::SpriteNode()
    : m_actorId(0)
    , m_x(0.0f)
    , m_y(0.0f)
    , m_rot(0.0f)
    , m_flippedVert(false)
    , m_flippedHori(false)
{
}

void SpriteNode::setImage(const Image &image)
{
    m_image.reset(new Image(image));
}
