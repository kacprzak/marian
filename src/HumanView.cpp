#include "HumanView.h"
#include "ResourceMgr.h"
#include "Engine.h"
#include "Util.h"

#include "components/RenderComponent.h"
#include "graphics/SpriteNode.h"

#define DIRTY_HACK 1
#if DIRTY_HACK
#include "Game.h"
#endif

HumanView::HumanView()
    : m_heroId(0)
{
    // Read map from file
    m_map.loadFromFile("media/map2.tmx");

    // Load map images
    auto images = m_map.externalImages();
    for (const std::string& image : images)
        ResourceMgr::singleton().addTexture(image);

    // Build objects
    std::vector<MapObject> mapObjects;
    m_map.getObjects(mapObjects);

    //std::cout << "INFO: " << mapObjects.size() << " MapObjects loaded.\n";

    unsigned long actorId = 0;

    for (const MapObject& obj : mapObjects) {

        ++actorId;

        if (obj.type == "Box") {
            SpriteNode *sprite = new SpriteNode();

            const Texture *tex =
                ResourceMgr::singleton().getTexture("minecraft_tiles_big.png");
            Image img(tex, 256, 480, 288, 512);

            sprite->m_actorId = actorId;
            sprite->setImage(img);

            m_nodes.insert(std::make_pair(actorId, sprite));
        } else if (obj.type == "Hero") {
            m_heroId = actorId;
        }
    }

    elh.registerListener(ACTOR_MOVED, std::bind(&HumanView::handleActorMoved, this, std::placeholders::_1));
}

//------------------------------------------------------------------------------

HumanView::~HumanView()
{
    for (const auto& pair : m_nodes) {
        delete pair.second;
    }
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

        if (a->category() == BOX) {
            SpriteNode *sprite = m_nodes[a->id()];
            if (sprite) {
                auto img = sprite->m_image;
                if (img)
                    e->drawImage(*img, sprite->m_x, sprite->m_y, sprite->m_rot);
            }
        } else {
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

    if (e->m_actor == m_heroId) {
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
    } else {
        SpriteNode *sprite = m_nodes[e->m_actor];
        if (sprite) {
            sprite->m_x = e->m_x;
            sprite->m_y = e->m_y;
            sprite->m_rot = e->m_angle;
        }
    }
}
