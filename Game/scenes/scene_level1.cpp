#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include "system_resources.h"
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
        auto playerRect = new IntRect( {0,0}, Vector2i(32,32) );

        player->setPosition(startPos);
        auto idleTexture = Resources::get<Texture>("Free/Main Characters/Mask Dude/Idle (32x32).png");
        auto psprite = player->addComponent<SpriteComponent>();
        psprite->setTexture(idleTexture);
        psprite->getSprite().setOrigin(16.f, 16.f);
//        psprite->getSprite().setScale(1.5f, 1.5f);
        auto animComp = player->addComponent<AnimationComponent>();
        animComp->setAnimation(11, .05, idleTexture, *playerRect);
        player->addComponent<PlayerPhysicsComponent>(Vector2f(16.f, 30));
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
      e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
    }
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
