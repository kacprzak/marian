#ifndef SPRITENODE_H
#define SPRITENODE_H

#include "Actor.h"
#include "Image.h"

#include <memory>

class SpriteNode
{
public:
    SpriteNode();

    void setImage(const Image& image);

//private:
    ActorId m_actorId;
    std::shared_ptr<Image> m_image;

    float m_x;
    float m_y;

    float m_rot;

    bool m_flippedVert;
    bool m_flippedHori;

};

#endif // SPRITENODE_H
