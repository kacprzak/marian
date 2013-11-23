#ifndef SPRITENODE_H
#define SPRITENODE_H

#include "actors/Actor.h"
#include "Image.h"
#include "components/PhysicsComponent.h"

namespace gfx {

class SpriteNode
{
public:
    SpriteNode();
    virtual ~SpriteNode() { delete m_image; }

    virtual void update(float /*elapsedTime*/) {}

    void setImage(const Image& image);
    const Image *image() { return m_image; }

    void setActorId(ActorId aid) { m_actorId = aid; }
    ActorId actorId() const { return m_actorId; }

    float x() const { return m_x; }
    float y() const { return m_y; }
    float angle() const { return m_rot; }

    virtual void moveTo(float x, float y, float angle = 0.0f);
    virtual void changePhysicsState(ActorPhysicsStateId /*newState*/) {}

    virtual void flipHorizontally(bool flip);
    virtual void flipVertically(bool flip);

protected:
    ActorId m_actorId;
    const Image *m_image;

    float m_x;
    float m_y;

    float m_rot;

    bool m_flippedVert;
    bool m_flippedHori;

};

} // namespace gfx

#endif // SPRITENODE_H
