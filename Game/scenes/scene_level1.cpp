#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include "system_resources.h"
#include "../components/cmp_particle_generator.h"
#include "../components/cmp_floating_platform.h"
#include "../components/cmp_hazard.h"
#include "../components/cmp_hud.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level1Scene::Load() {
    cout << " Scene 1 Load" << endl;
    ls::loadTextureFile("res/img/Free/Terrain/Terrain (16x16).png", 16);
    ls::loadLevelFile("res/level1.json", 40.0f);



  // Create player
    {
        player = makeEntity();
        auto startPos = ls::getTilePosition(ls::findTiles(ls::START)[0]);

        player->setPosition(startPos);
        auto idleTexture = Resources::get<Texture>("Free/Main Characters/Mask Dude/Idle (32x32).png");
        auto psprite = player->addComponent<SpriteComponent>();
        psprite->setTexture(idleTexture);
        psprite->getSprite().setOrigin(16.f, 16.f);

        auto animComp = player->addComponent<AnimationComponent>();
        auto pPhys = player->addComponent<PlayerPhysicsComponent>(Vector2f(16.f, 30));

        // Setting up hud
        auto hud = makeEntity();
        hud->addComponent<HudComponent>(pPhys);

    }

  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
    auto floors = ls::findTiles(ls::FLOOR);
    walls.insert(walls.end(), floors.begin(), floors.end());
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(20.f, 20.f); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(b2_staticBody, Vector2f(40.f, 40.f));
    }
  }

  // Add in particle generator
    {
        auto particle = makeEntity();
        auto pos = ls::getTilePosition(ls::findTiles(ls::PARTICLEGENERATOR)[0]);
        particle->addComponent<ParticleGenerator>(10, 2.f, 100.f, 100.f, pos, Resources::get<Texture>("Free/Traps/Sand Mud Ice/Ice Particle.png"), .1, true);
    }

    //Add in floating platform
    {
        int platformCount = 3;
        for (int i = 0; i < platformCount; i++) {
            auto platform = makeEntity();
            auto startPos = ls::getTilePosition(ls::findTiles(ls::WAYPOINT)[0]) + Vector2f(i*40.f, 0.f);
            auto endPos = ls::getTilePosition(ls::findTiles(ls::WAYPOINT)[1]) + Vector2f(i*40.f, 0.f);
            auto platformTexture = Resources::get<Texture>("Free/Terrain/Terrain (16x16).png");
            IntRect platformRect = IntRect(272, 144, 16, 16);
            auto psprite = platform->addComponent<SpriteComponent>();
            psprite->setTexture(platformTexture);
            psprite->getSprite().setTextureRect(platformRect);
            psprite->getSprite().setScale(2.5f, 2.5f);
            psprite->getSprite().setOrigin(8.f, 8.f);
            platform->addComponent<FloatingPlatformComponent>(startPos, endPos, 50.f);
            platform->addComponent<PhysicsComponent>(b2_kinematicBody, Vector2f(40.f, 40.f));
        }
    }

    //TODO - !!!!Separate hazards from level loader and add them in here with new hazard component!!!
    {
        auto hazard = makeEntity();
        auto pos = ls::getTilePosition(ls::findTiles(ls::HAZARD)[0]);
        hazard->setPosition(pos);
        auto hazardTexture = Resources::get<Texture>("Free/Traps/Spikes/Idle.png");
        auto hsprite = hazard->addComponent<SpriteComponent>();
        hsprite->setTexture(hazardTexture);
        //hsprite->getSprite().setOrigin(16.f, 16.f);
        hsprite->getSprite().setScale(2.5f, 2.5f);
        auto hHazard = hazard->addComponent<HazardComponent>(10, player);

    }

  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

  if (ls::getTileAt(player->getPosition()) == ls::END) {
    Engine::ChangeScene((Scene*)&menu);
    return;
  }
  Scene::Update(dt);
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
