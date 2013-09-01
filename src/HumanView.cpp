#include "HumanView.h"
#include "ResourceMgr.h"
#include "Engine.h"
#include "Util.h"
#include "Map.h"

#include "components/RenderComponent.h"
#include "graphics/SpriteNode.h"
#include "graphics/HeroNode.h"
#include "graphics/MapNode.h"
#include "input/HeroController.h"

HumanView::HumanView()
    : m_heroId(0)
    , m_keyboardHandler(new HeroController)
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

            static_cast<HeroController *>(m_keyboardHandler)->setActor(actorId);
        }
    }

    elh.registerListener(ACTOR_MOVED, std::bind(&HumanView::handleActorMoved, this, std::placeholders::_1));
    elh.registerListener(ACTOR_PHYSICS_STATE_CHANGED, std::bind(&HumanView::handleActorPhysicsStateChanged, this, std::placeholders::_1));
    elh.registerListener(ACTOR_CREATED, std::bind(&HumanView::handleActorCreated, this, std::placeholders::_1));
    elh.registerListener(ACTOR_DESTROYED, std::bind(&HumanView::handleActorDestroyed, this, std::placeholders::_1));
}

//------------------------------------------------------------------------------

HumanView::~HumanView()
{
    for (const auto& pair : m_nodes) {
        delete pair.second;
    }

    delete m_keyboardHandler;
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
        m_keyboardHandler->onKeyUp(event.key.keysym.scancode);
        break;
    case SDL_KEYDOWN:
        m_keyboardHandler->onKeyDown(event.key.keysym.scancode);
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

    m_mapNode.update(elapsedTime);
}

//------------------------------------------------------------------------------

void HumanView::draw(Engine *e)
{
    // Draw only visible part of the map
    ViewRect r;
    e->viewBounds(&r);

    m_mapNode.drawBackground(e, r);

    // Draw actors
    for (auto& pair : m_nodes) {
        SpriteNode *sprite = pair.second;
        const Image *img = nullptr;
        if (sprite && (img = sprite->image())) {
            e->drawImage(*img, sprite->x(), sprite->y(), sprite->angle());
        }
    }

    m_mapNode.drawForeground(e, r);
}

//------------------------------------------------------------------------------

void HumanView::handleActorMoved(EventPtr event)
{
    auto e = std::static_pointer_cast<MoveEvent>(event);

    //ActorPtr actor = m_actors[e->m_actor];

    if (e->m_actorId == m_heroId) {
        float x = e->m_x;
        float y = e->m_y;

        // Respect map borders
        ViewRect r;
        Engine::singleton().viewBounds(&r);

        float hw = (r.right - r.left) / 2.0f;
        float hh = (r.top - r.bottom) / 2.0f;
        if (x < hw) x = hw;
        if (y < hh) y = hh;
        if (x > m_mapNode.width() - hw) x = m_mapNode.width() - hw;

        Engine::singleton().centerViewOn(x, y);
    }

    SpriteNode *sprite = m_nodes[e->m_actorId];
    if (sprite) {
        sprite->moveTo(e->m_x, e->m_y, e->m_angle);
    }
}

//------------------------------------------------------------------------------

void HumanView::handleActorPhysicsStateChanged(EventPtr event)
{
    auto e = std::static_pointer_cast<PhysicsStateChangeEvent>(event);

    SpriteNode *sprite = m_nodes[e->m_actorId];
    if (sprite) {
        sprite->changePhysicsState(e->m_newState);
    }
}

//------------------------------------------------------------------------------

void HumanView::handleActorCreated(EventPtr event)
{
    auto e = std::static_pointer_cast<ActorCreatedEvent>(event);

    if (e->m_actorCategory == BOX) {
        SpriteNode *sprite = new SpriteNode();

        const Texture *tex = ResourceMgr::singleton().getTexture("minecraft_tiles_big.png");
        Image img(tex, 256, 480, 288, 512);

        sprite->setActorId(e->m_actorId);
        sprite->setImage(img);

        m_nodes.insert(std::make_pair(e->m_actorId, sprite));
    }
}

//------------------------------------------------------------------------------

void HumanView::handleActorDestroyed(EventPtr event)
{
    auto e = std::static_pointer_cast<ActorDestroyedEvent>(event);

    SpriteNode *sprite = m_nodes[e->m_actorId];
    if (sprite) {
        m_nodes.erase(e->m_actorId);
        delete sprite;
    }
}
