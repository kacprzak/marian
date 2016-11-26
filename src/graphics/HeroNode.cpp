#include "HeroNode.h"

#include "Animation.h"
#include "ResourceMgr.h"

namespace gfx {

//==============================================================================

class HeroNodeState : public State<HeroNode*>
{
  public:
    explicit HeroNodeState(HeroNodeStateMachine& stateMachine)
        : State<HeroNode*>()
        , m_stateMachine(stateMachine)
    {
    }

    void onEnter(HeroNode* owner, int /*prevStateId*/) override
    {
        setFacingRight(owner->isFacingRight());
    }

    void onExit(HeroNode* /*owner*/, int /*nextStateId*/) override {}

    virtual void update(float elapsedTime)    = 0;
    virtual const Image& currentImage() const = 0;
    virtual void setFacingRight(bool right)   = 0;

  protected:
    HeroNodeStateMachine& m_stateMachine;
};

//==============================================================================

class StandHeroState : public HeroNodeState
{
  public:
    explicit StandHeroState(HeroNodeStateMachine& stateMachine)
        : HeroNodeState(stateMachine)
    {
        const Texture* tex =
            ResourceMgr::singleton().getTexture("MegaMan_001.png");

        Rect<int> tileCoords = Rect<int>(38, 255, 38 + 32, 255 + 32);

        m_animation.setReversable(true);
        Image idleFrame1(tex, tileCoords);
        idleFrame1.scale(2.0f);
        m_animation.addFrame(idleFrame1, 1.0f);

        tileCoords.left += 36;
        tileCoords.right += 36;
        Image idleFrame2(tex, tileCoords);
        idleFrame2.scale(2.0f);
        m_animation.addFrame(idleFrame2, 0.1f); // Blink
    }

    void update(float elapsedTime) override { m_animation.update(elapsedTime); }

    const Image& currentImage() const override
    {
        return m_animation.currentFrame();
    }

    void setFacingRight(bool right) override
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
    explicit FallHeroState(HeroNodeStateMachine& stateMachine)
        : HeroNodeState(stateMachine)
    {
        const Texture* tex =
            ResourceMgr::singleton().getTexture("MegaMan_001.png");

        Rect<int> tileCoords = Rect<int>(182, 219, 182 + 32, 219 + 32);

        m_image = std::make_unique<Image>(tex, tileCoords);
        m_image->scale(2.0f);
    }

    void update(float /*elapsedTime*/) override
    {
        // Do nothing
    }

    const Image& currentImage() const override { return *m_image; }

    void setFacingRight(bool right) override
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
    explicit RunHeroState(HeroNodeStateMachine& stateMachine)
        : HeroNodeState(stateMachine)
    {
        const Texture* tex =
            ResourceMgr::singleton().getTexture("MegaMan_001.png");

        float frameSpeed = 0.25f;
        m_animation.setReversable(true);

        const Rect<int> tileCoords = Rect<int>(74, 183, 74 + 32, 183 + 32);
        const int ax_off           = 36;
        const int ay_off           = 36;

        const int y = 1;
        for (int x = 2; x >= 0; --x) {
            Rect<int> tc = tileCoords;

            tc.left += ax_off * x;
            tc.right += ax_off * x;
            tc.top += ay_off * y;
            tc.bottom += ay_off * y;

            Image runFrame(tex, tc);
            runFrame.scale(2.0f);
            float fs = frameSpeed;
            // Make last and first shorter
            if (x == 0 || x == 2)
                fs = frameSpeed / 2.0f;
            m_animation.addFrame(runFrame, fs);
        }
    }

    void update(float elapsedTime) override { m_animation.update(elapsedTime); }

    const Image& currentImage() const override
    {
        return m_animation.currentFrame();
    }

    void setFacingRight(bool right) override
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
    HeroNodeState* state = new StandHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(IDLE, state);

    state = new RunHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(RUN, state);

    state = new FallHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(FALL, state);

    m_stateMachine.changeState(IDLE);
}

//------------------------------------------------------------------------------

HeroNode::~HeroNode()
{
    for (auto* state : m_states)
        delete state;

    m_image = nullptr;
}

//------------------------------------------------------------------------------

void HeroNode::update(float elapsedTime)
{
    m_stateMachine.currentState()->update(elapsedTime);

    const HeroNodeState* renderState =
        static_cast<const HeroNodeState*>(m_stateMachine.currentState());
    m_image = &(renderState->currentImage());
}

//------------------------------------------------------------------------------

void HeroNode::moveTo(float x, float y, float angle)
{
    // Display Node a bit higher than actor
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

} // namespace gfx
