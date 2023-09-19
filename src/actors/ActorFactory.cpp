#include "ActorFactory.h"

#include "../components/ActorComponent.h"

#include "../actors/Box.h"
#include "../actors/Ground.h"
#include "../actors/Hero.h"
#include "../actors/Sensor.h"

#include <cassert>
#include <memory>

std::unique_ptr<Actor> ActorFactory::create(GameLogic* game,
                                            const MapObject& obj)
{
    assert(game);
    auto actor = std::make_unique<Actor>(getNextId(), game);

    if (obj.type == "Hero") {
        actor->setCategory(HERO);

        auto physics = std::make_shared<HeroPhysicsComponent>(
            game, obj.x, obj.y, obj.width, obj.height);
        actor->addComponent(physics);
        physics->init();

        auto render = std::make_shared<HeroRenderComponent>();
        actor->addComponent(render);
        render->init();

    } else if (obj.type == "Box") {
        actor->setCategory(BOX);

        auto physics = std::make_shared<BoxPhysicsComponent>(
            game, obj.x, obj.y, obj.width, obj.height);
        actor->addComponent(physics);
        physics->init();

        auto render = std::make_shared<RenderComponent>();
        actor->addComponent(render);
        render->init();

    } else if (obj.type == "Sensor") {
        actor->setCategory(SENSOR);

        auto physics = std::make_shared<SensorPhysicsComponent>(game, obj);
        actor->addComponent(physics);
        physics->init();

    } else {
        // Static collision shape
        actor->setCategory(GROUND);

        auto physics = std::make_shared<GroundPhysicsComponent>(game, obj);
        actor->addComponent(physics);
        physics->init();
    }

    actor->setName(obj.name);
    return actor;
}

//--------------------------------------------------------------------------

std::unique_ptr<Actor> ActorFactory::create(GameLogic* game, ActorCategory type,
                                            const std::string& name, float x,
                                            float y)
{
    assert(game);
    auto actor = std::make_unique<Actor>(getNextId(), game);

    if (type == BOX) {
        actor->setCategory(BOX);

        auto physics = std::make_shared<BoxPhysicsComponent>(game, x, y);
        actor->addComponent(physics);
        physics->init();

        auto render = std::make_shared<RenderComponent>();
        actor->addComponent(render);
        render->init();
    }

    actor->setName(name);
    return actor;
}

//--------------------------------------------------------------------------

unsigned long ActorFactory::getNextId()
{
    static unsigned long id = 0;
    ++id;
    return id;
}
