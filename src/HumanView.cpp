#include "HumanView.h"
#include "ResourceMgr.h"
#include "Engine.h"
#include "Util.h"

#include "components/RenderComponent.h"

#define DIRTY_HACK 1
#if DIRTY_HACK
#include "Game.h"
#endif

HumanView::HumanView()
{
    // Read map from file
    m_map.loadFromFile("media/map2.tmx");

    elh.registerListener(ACTOR_MOVED, std::bind(&HumanView::handleActorMoved, this, std::placeholders::_1));
}

//------------------------------------------------------------------------------

void HumanView::initialize(Engine *e)
{
    // Set background color
    std::string bgColor = m_map.backgroundColor();
    if (!bgColor.empty()) {
        std::vector<int> color = hexColorToRgb(bgColor);
        e->setBackgroundColor(color[0], color[1], color[2]);
    }
}

//------------------------------------------------------------------------------

bool HumanView::processInput(const SDL_Event &event)
{
    switch (event.type) {
    case SDL_KEYUP:
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) return false;
        break;
    }
    return true; // keep running
}

//------------------------------------------------------------------------------

void HumanView::update(float /*elapsedTime*/)
{
}

//------------------------------------------------------------------------------

void HumanView::draw(Engine *e)
{
    // Draw only visible part of the map
    float x1, x2, y1, y2;
    e->viewBounds(&x1, &x2, &y1, &y2);

    // TODO: read order from map
    m_map.drawLayer(e, "back",   x1, x2, y1, y2);
    m_map.drawLayer(e, "ground", x1, x2, y1, y2);

#if DIRTY_HACK
    Game *game = static_cast<Game*>(e->game());

    // Draw actors
    for (const auto& pair : game->actors()) {
        ActorPtr a = pair.second;
        // Get weak_ptr
        auto rcwp = a->getComponent<RenderComponent>(RENDER);
        // Try to get shared_ptr
        if (auto rcsp = rcwp.lock()) {
            auto pcwp = a->getComponent<PhysicsComponent>(PHYSICS);
            if (auto pcsp = pcwp.lock()) {
                e->drawImage(rcsp->currentImage(),
                             pcsp->posX() + rcsp->xOffset(),
                             pcsp->posY() + rcsp->yOffset(),
                             pcsp->angle());
            }
        }
    }
#endif

    m_map.drawLayer(e, "water", x1, x2, y1, y2);
    m_map.drawLayer(e, "front", x1, x2, y1, y2);
}

//------------------------------------------------------------------------------

void HumanView::handleActorMoved(EventPtr event)
{
    auto e = std::static_pointer_cast<MoveEvent>(event);

    //ActorPtr actor = m_actors[e->m_actor];

    if (e->m_actor == 1) {
        float x = e->m_x;
        float y = e->m_y;

        // Respect map borders
        float bLeft, bRight, bTop, bBottom;
        Engine::singleton().viewBounds(&bLeft, &bRight, &bBottom, &bTop);

        float hw = (bRight - bLeft) / 2.0f;
        float hh = (bTop - bBottom) / 2.0f;
        if (x < hw) x = hw;
        if (y < hh) y = hh;
        if (x > m_map.width() - hw) x = m_map.width() - hw;

        Engine::singleton().centerViewOn(x, y);
    }
}
