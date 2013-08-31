#include "HeroNode.h"

#include "Animation.h"
#include "ResourceMgr.h"

//==============================================================================

class HeroNodeState : public State<HeroNode *>
{
 public:
    HeroNodeState(HeroNodeStateMachine& stateMachine)
        : State<HeroNode *>()
        , m_stateMachine(stateMachine)
    {}

    void onEnter(HeroNode * owner, int /*prevStateId*/) override
    {
        setFacingRight(owner->isFacingRight());
    }

    virtual void onExit(HeroNode * /*owner*/, int /*nextStateId*/) override {}

    virtual void update(Engine *e, float elapsedTime) = 0;

    virtual const Image& currentImage() const = 0;

    virtual void setFacingRight(bool right) = 0;

 protected:
    HeroNodeStateMachine& m_stateMachine;
};

//==============================================================================

class StandHeroState : public HeroNodeState
{
 public:
    StandHeroState(HeroNodeStateMachine& stateMachine)
        : HeroNodeState(stateMachine)
    {
        const Texture *tex = ResourceMgr::singleton().getTexture("MegaMan_001.png");

        int ax = 38;
        int ax_off = 36;
        int ay = 255;

        m_animation.setReversable(true);
        Image idleFrame1(tex, ax, ay, ax + 32, ay + 32);
        idleFrame1.scale(2.0f);
        m_animation.addFrame(idleFrame1, 1.0f);
        Image idleFrame2(tex, ax + ax_off, ay, ax + ax_off + 32, ay + 32);
        idleFrame2.scale(2.0f);
        m_animation.addFrame(idleFrame2, 0.1f); // Blink
    }

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);
    }

    const Image& currentImage() const override
    {
        return m_animation.currentFrame();
    }

    void setFacingRight(bool right)
    {
        static bool animFacingRight = true;
        if (animFacingRight != right) {
            m_animation.flipVertically();
            animFacingRight = right;
        }
    }

    Animation m_animation;
};

//==============================================================================

class FallHeroState : public HeroNodeState
{
 public:
    FallHeroState(HeroNodeStateMachine& stateMachine)
        : HeroNodeState(stateMachine)
    {
        const Texture *tex = ResourceMgr::singleton().getTexture("MegaMan_001.png");

        int ax = 38;
        int ax_off = 4*36;
        int ay = 219;

        m_image = std::unique_ptr<Image>(new Image(tex, ax + ax_off, ay, ax + ax_off + 32, ay + 32));
        m_image->scale(2.0f);
    }

    void update(Engine * /*e*/, float /*elapsedTime*/) override
    {
        // Do nothing
    }

    const Image& currentImage() const override
    {
        return *m_image;
    }

    void setFacingRight(bool right)
    {
        static bool imageFacingRight = true;
        if (imageFacingRight != right) {
            m_image->flipVertically();
            imageFacingRight = right;
        }
    }

    std::unique_ptr<Image> m_image;
};

//==============================================================================

class RunHeroState : public HeroNodeState
{
 public:
    RunHeroState(HeroNodeStateMachine& stateMachine)
        : HeroNodeState(stateMachine)
    {
        const Texture *tex = ResourceMgr::singleton().getTexture("MegaMan_001.png");

        int ax = 38 + 36;
        int ax_off = 36;
        int ay_off = 36;
        int ay = 219 - 36;
        float frameSpeed = 0.25f;
        m_animation.setReversable(true);

        int y = 1;
        for (int x = 2 ; x >= 0; --x) {
            Image runFrame(tex,
                           ax + (ax_off * x),      ay + (ay_off * y),
                           ax + (ax_off * x) + 32, ay + (ay_off * y) + 32);
            runFrame.scale(2.0f);
            float fs = frameSpeed;
            // Make last and first shorter
            if (x == 0 || x == 2) fs = frameSpeed / 2.0f;
            m_animation.addFrame(runFrame, fs);
        }
    }

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);
    }

    const Image& currentImage() const override
    {
        return m_animation.currentFrame();
    }

    void setFacingRight(bool right)
    {
        static bool animFacingRight = true;
        if (animFacingRight != right) {
            m_animation.flipVertically();
            animFacingRight = right;
        }
    }

    Animation m_animation;
};

//==============================================================================

HeroNode::HeroNode()
    : m_facingRight(true)
    , m_stateMachine(nullptr, 0)
{
    m_stateMachine.setOwner(this);

    // States
    HeroNodeState *state = new StandHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(STAND, state);

    state = new RunHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(RUN, state);

    state = new FallHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(FALL, state);

    m_stateMachine.changeState(STAND);
}

//------------------------------------------------------------------------------

HeroNode::~HeroNode()
{
    for (auto *state : m_states)
        delete state;

    m_image = nullptr;
}

//------------------------------------------------------------------------------

void HeroNode::update(float elapsedTime)
{
    m_stateMachine.currentState()->update(nullptr, elapsedTime);

    const HeroNodeState *renderState = static_cast<const HeroNodeState*>(m_stateMachine.currentState());
    m_image = &(renderState->currentImage());
}

//------------------------------------------------------------------------------

void HeroNode::moveTo(float x, float y, float angle)
{
    SpriteNode::moveTo(x, y + 0.5f, angle);
}

//------------------------------------------------------------------------------

void HeroNode::changePhysicsState(ActorPhysicsStateId newState)
{
    m_stateMachine.changeState(newState);
}

//------------------------------------------------------------------------------

void HeroNode::setFacingRight(bool right)
{
    m_facingRight = right;
    m_stateMachine.currentState()->setFacingRight(right);
}


