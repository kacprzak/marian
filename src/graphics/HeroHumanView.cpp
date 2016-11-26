/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#include "HeroHumanView.h"

#include "Engine.h"
#include "GLRenderer.h"
#include "Map.h"
#include "ResourceMgr.h"
#include "Util.h"

#include "components/RenderComponent.h"
#include "graphics/HeroNode.h"
#include "graphics/MapNode.h"
#include "graphics/SpriteNode.h"
#include "input/HeroController.h"

using namespace gfx;
using namespace event;

HeroHumanView::HeroHumanView(const std::string &title, int screenWidth,
                             int screenHeight, bool screenFull)
    : HumanView(title, screenWidth, screenHeight, screenFull),
      m_renderer(new GLRenderer), m_heroId(0),
      m_keyboardHandler(new HeroController)
{
    std::shared_ptr<Map> map(new Map);
    ResourceMgr &resourceMgr = ResourceMgr::singleton();
    // Read map from file
    std::string assetsFolder = resourceMgr.dataFolder();
    map->loadFromFile(assetsFolder + "map2.tmx");

    // Load map images
    auto images = map->externalImages();
    for (const std::string &image : images)
        resourceMgr.addTexture(image);

    // Build objects
    std::vector<MapObject> mapObjects;
    map->getObjects(mapObjects);

    // std::cout << "INFO: " << mapObjects.size() << " MapObjects loaded.\n";
    // Hero texture
    resourceMgr.addTexture("MegaMan_001.png");

    m_mapNode.setMap(map);

    unsigned long actorId = 0;

    for (const MapObject &obj : mapObjects) {

        ++actorId;

        if (obj.type == "Box") {
            const Texture *tex =
                resourceMgr.getTexture("minecraft_tiles_big.png");
            Image img(tex, Rect<int>(256, 480, 288, 512));

            SpriteNode *sprite = new SpriteNode();
            sprite->setActorId(actorId);
            sprite->setImage(img);

            m_nodes.insert(std::make_pair(actorId, sprite));

        } else if (obj.type == "Hero") {
            m_heroId = actorId;

            SpriteNode *sprite = new HeroNode();
            sprite->setActorId(actorId);

            m_nodes.insert(std::make_pair(actorId, sprite));
            m_keyboardHandler->setActor(actorId);
        }
    }

    // Set background color
    setBackgroundColor(m_mapNode.backgroundColor());

    elh.registerListener(ACTOR_MOVED,
                         std::bind(&HeroHumanView::handleActorMoved, this,
                                   std::placeholders::_1));
    elh.registerListener(
        ACTOR_PHYSICS_STATE_CHANGED,
        std::bind(&HeroHumanView::handleActorPhysicsStateChanged, this,
                  std::placeholders::_1));
    elh.registerListener(ACTOR_CREATED,
                         std::bind(&HeroHumanView::handleActorCreated, this,
                                   std::placeholders::_1));
    elh.registerListener(ACTOR_DESTROYED,
                         std::bind(&HeroHumanView::handleActorDestroyed, this,
                                   std::placeholders::_1));
    elh.registerListener(INPUT_COMMAND,
                         std::bind(&HeroHumanView::handleInputCommand, this,
                                   std::placeholders::_1));
}

//------------------------------------------------------------------------------

HeroHumanView::~HeroHumanView()
{
    for (const auto &pair : m_nodes) {
        delete pair.second;
    }
}

//------------------------------------------------------------------------------

bool HeroHumanView::processInput(const SDL_Event &event)
{
    if (super::processInput(event))
        return false;

    switch (event.type) {
    case SDL_KEYUP:
        m_keyboardHandler->onKeyUp(event.key.keysym.scancode);
        break;
    case SDL_KEYDOWN:
        m_keyboardHandler->onKeyDown(event.key.keysym.scancode);
        break;
    }
    return false; // dont intercept event
}

//------------------------------------------------------------------------------

void HeroHumanView::update(float elapsedTime)
{
    super::update(elapsedTime);

    for (auto &pair : m_nodes) {
        SpriteNode *s = pair.second;
        if (s)
            s->update(elapsedTime);
    }

    m_mapNode.update(elapsedTime);
}

//------------------------------------------------------------------------------

void HeroHumanView::draw()
{
    preDraw();

    // Draw only visible part of the map
    ViewRect r;
    viewBounds(&r);

    m_mapNode.drawBackground(m_renderer.get(), r);

    // Draw actors
    for (auto &pair : m_nodes) {
        SpriteNode *sprite = pair.second;
        const Image *img   = nullptr;
        if (sprite && (img = sprite->image())) {
            m_renderer->drawImage(*img, sprite->x(), sprite->y(),
                                  sprite->angle());
        }
    }

    m_mapNode.drawForeground(m_renderer.get(), r);

    postDraw();
}

//------------------------------------------------------------------------------

void HeroHumanView::handleActorMoved(Event &event)
{
    auto &e = static_cast<MoveEvent &>(event);

    // ActorPtr actor = m_actors[e->m_actor];

    if (e.m_actorId == m_heroId) {
        float x = e.m_x;
        float y = e.m_y;

        // Respect map borders
        ViewRect r;
        viewBounds(&r);

        float hw = (r.right - r.left) / 2.0f;
        float hh = (r.top - r.bottom) / 2.0f;
        if (x < hw)
            x = hw;
        if (y < hh)
            y = hh;
        if (x > m_mapNode.width() - hw)
            x = m_mapNode.width() - hw;

        centerViewOn(x, y);
    }

    SpriteNode *sprite = m_nodes[e.m_actorId];
    if (sprite) {
        sprite->moveTo(e.m_x, e.m_y, e.m_angle);
    }
}

//------------------------------------------------------------------------------

void HeroHumanView::handleActorPhysicsStateChanged(Event &event)
{
    auto &e = static_cast<PhysicsStateChangeEvent &>(event);

    SpriteNode *sprite = m_nodes[e.m_actorId];
    if (sprite) {
        sprite->changePhysicsState(e.m_newState);
    }
}

//------------------------------------------------------------------------------

void HeroHumanView::handleActorCreated(Event &event)
{
    auto &e = static_cast<ActorCreatedEvent &>(event);

    if (e.m_actorCategory == BOX) {

        const Texture *tex =
            ResourceMgr::singleton().getTexture("minecraft_tiles_big.png");
        Image img(tex, Rect<int>(256, 480, 288, 512));

        SpriteNode *sprite = new SpriteNode();
        sprite->setActorId(e.m_actorId);
        sprite->setImage(img);

        m_nodes.insert(std::make_pair(e.m_actorId, sprite));
    }
}

//------------------------------------------------------------------------------

void HeroHumanView::handleActorDestroyed(Event &event)
{
    auto &e = static_cast<ActorDestroyedEvent &>(event);

    SpriteNode *sprite = m_nodes[e.m_actorId];

    if (sprite) {
        m_nodes.erase(e.m_actorId);
        delete sprite;
    }
}

//------------------------------------------------------------------------------

void HeroHumanView::handleInputCommand(Event &event)
{
    auto &e = static_cast<ActorInputEvent &>(event);

    SpriteNode *sprite = m_nodes[e.m_actorId];

    if (sprite && (e.m_actorId == m_heroId)) {
        if (e.m_command == MOVE_RIGHT_START)
            sprite->flipHorizontally(false);
        if (e.m_command == MOVE_LEFT_START)
            sprite->flipHorizontally(true);
    }
}
