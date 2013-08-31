#include "HumanView.h"
#include "ResourceMgr.h"
#include "Engine.h"
#include "Util.h"

#include "components/RenderComponent.h"
#include "graphics/SpriteNode.h"
#include "graphics/HeroNode.h"
#include "graphics/MapNode.h"

#define DIRTY_HACK 1
#if DIRTY_HACK
#include "Game.h"
#endif

HumanView::HumanView()
    : m_heroId(0)
{
    std::shared_ptr<Map> map(new Map);
    // Read map from file
    map->loadFromFile("media/map2.tmx");

    // Load map images
    auto images = map->externalImages();
    for (const std::string& image : images)
        ResourceMgr::singleton().addTexture(image);

    // Build objects
    std::vector<MapObject> mapObjects;
    map->getObjects(mapObjects);

    //std::cout << "INFO: " << mapObjects.size() << " MapObjects loaded.\n";
    // Hero texture
    ResourceMgr::singleton().addTexture("MegaMan_001.png");

    m_mapNode.setMap(map);

    unsigned long actorId = 0;

    for (const MapObject& obj : mapObjects) {

        ++actorId;

        if (obj.type == "Box") {
            SpriteNode *sprite = new SpriteNode();

            const Texture *tex = ResourceMgr::singleton().getTexture("minecraft_tiles_big.png");
            Image img(tex, 256, 480, 288, 512);

            sprite->setActorId(actorId);
            sprite->setImage(img);

            m_nodes.insert(std::make_pair(actorId, sprite));
        } else if (obj.type == "Hero") {
            m_heroId = actorId;

            SpriteNode *sprite = new HeroNode();

            sprite->setActorId(actorId);
            m_nodes.insert(std::make_pair(actorId, sprite));
        }
    }

    elh.registerListener(ACTOR_MOVED, std::bind(&HumanView::handleActorMoved, this, std::placeholders::_1));
    elh.registerListener(ACTOR_PHYSICS_STATE_CHANGED, std::bind(&HumanView::handleActorPhysicsStateChanged, this, std::placeholders::_1));
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
    std::string bgColor = m_mapNode.backgroundColor();
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

void HumanView::update(float elapsedTime)
{
    for (auto& pair : m_nodes) {
        SpriteNode *s = pair.second;
        if (s)
            s->update(elapsedTime);
    }
}

//------------------------------------------------------------------------------

void HumanView::draw(Engine *e)
{
    // Draw only visible part of the map
    float x1, x2, y1, y2;
    e->viewBounds(&x1, &x2, &y1, &y2);

    // TODO: read order from map
    m_mapNode.drawLayer(e, "back",   x1, x2, y1, y2);
    m_mapNode.drawLayer(e, "ground", x1, x2, y1, y2);

    // Draw actors
    for (auto& pair : m_nodes) {
        SpriteNode *sprite = pair.second;
        const Image *img = nullptr;
        if (sprite && (img = sprite->image())) {
            e->drawImage(*img, sprite->x(), sprite->y(), sprite->angle());
        }
    }

    m_mapNode.drawLayer(e, "water", x1, x2, y1, y2);
    m_mapNode.drawLayer(e, "front", x1, x2, y1, y2);
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
        if (x > m_mapNode.width() - hw) x = m_mapNode.width() - hw;

        Engine::singleton().centerViewOn(x, y);
    }

    SpriteNode *sprite = m_nodes[e->m_actor];
    if (sprite) {
        sprite->moveTo(e->m_x, e->m_y, e->m_angle);
    }
}

//------------------------------------------------------------------------------

void HumanView::handleActorPhysicsStateChanged(EventPtr event)
{
    auto e = std::static_pointer_cast<PhysicsStateChangeEvent>(event);

    SpriteNode *sprite = m_nodes[e->m_actor];
    if (sprite) {
        sprite->changePhysicsState(e->m_newState);
    }
}
