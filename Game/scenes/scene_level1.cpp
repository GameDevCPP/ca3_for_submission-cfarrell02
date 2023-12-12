#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include "system_resources.h"
#include "../components/cmp_particle_generator.h"
#include "../components/cmp_floating_platform.h"
#include "../components/cmp_hazard.h"
#include "../components/cmp_pickup.h"
#include "../components/cmp_next_level.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;


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

        auto particle = makeEntity();
        auto pos = ls::getTilePosition(ls::findTiles(ls::PARTICLEGENERATOR)[0]);
        particle->addComponent<ParticleGenerator>(10, 2.f, 100.f, 100.f, pos, Resources::get<Texture>("Free/Traps/Sand Mud Ice/Ice Particle.png"), player, .1, true);
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

    {
        auto hazard = makeEntity();
        auto pos = ls::getTilePosition(ls::findTiles(ls::HAZARD)[0]);
//        pos = {pos.x + 40, pos.y + 40};
        hazard->setPosition(pos);
        auto hazardTexture = Resources::get<Texture>("Free/Traps/Spikes/Idle.png");
        auto hsprite = hazard->addComponent<SpriteComponent>();
        hsprite->setTexture(hazardTexture);
        //hsprite->getSprite().setOrigin(16.f, 16.f);
        hsprite->getSprite().setScale(2.5f, 2.5f);
        auto hHazard = hazard->addComponent<HazardComponent>(10, player);

    }
    {
        auto pickupLocations = ls::findTiles(ls::SCORE);
        for(auto p : pickupLocations){
            auto pos = ls::getTilePosition(p);
            pos = {pos.x + 20, pos.y + 30};
            auto pickup = makeEntity();
            pickup->setPosition(pos);
            cout<<"Pickup at: "<<pos.x<<", "<<pos.y<<endl;
            auto pickupTexture = Resources::get<Texture>("Free/Items/Fruits/Apple.png");
            cout<<"Pickup Texture: "<<pickupTexture->getSize().x<<", "<<pickupTexture->getSize().y<<endl;
            auto psprite = pickup->addComponent<SpriteComponent>();
            psprite->setTexture(pickupTexture);
            psprite->getSprite().setOrigin(16.f, 16.f);
            auto animComp = pickup->addComponent<AnimationComponent>();
            animComp->setAnimation(17, .1, pickupTexture, IntRect(Vector2i(0, 0), Vector2i(32, 32)));
            pickup->addComponent<PickupComponent>(PickupComponent::PickupType::SCORE, 10, player);
        }
    }
    {
        auto pickupLocations = ls::findTiles(ls::HEALTH);
        for(auto p : pickupLocations){
            auto pos = ls::getTilePosition(p);
            pos = {pos.x + 20, pos.y + 30};
            auto pickup = makeEntity();
            pickup->setPosition(pos);
            cout<<"Pickup at: "<<pos.x<<", "<<pos.y<<endl;
            auto pickupTexture = Resources::get<Texture>("Free/Items/Fruits/Pineapple.png");
            cout<<"Pickup Texture: "<<pickupTexture->getSize().x<<", "<<pickupTexture->getSize().y<<endl;
            auto psprite = pickup->addComponent<SpriteComponent>();
            psprite->setTexture(pickupTexture);
            psprite->getSprite().setOrigin(16.f, 16.f);
            auto animComp = pickup->addComponent<AnimationComponent>();
            animComp->setAnimation(17, .1, pickupTexture, IntRect(Vector2i(0, 0), Vector2i(32, 32)));
            pickup->addComponent<PickupComponent>(PickupComponent::PickupType::HEALTH, 10, player);
        }
    }

    //TODO End level point
    {
        auto end = makeEntity();
        auto pos = ls::getTilePosition(ls::findTiles(ls::ENDLEVEL)[0]);
        pos.y += 10; // Offset the flad to be on the ground
        end->setPosition(pos);
        auto endTexture = Resources::get<Texture>("Free/Items/Checkpoints/Checkpoint/Checkpoint (Flag Idle)(64x64).png");
        auto esprite = end->addComponent<SpriteComponent>();
        esprite->setTexture(endTexture);
        esprite->getSprite().setOrigin(32.f, 32.f);
        auto animComp = end->addComponent<AnimationComponent>();
        animComp->setAnimation(10, .1, endTexture, IntRect(Vector2i(0, 0), Vector2i(64, 64)));
        end->addComponent<NextLevelComponent >(player,std::make_shared<Level1Scene>(), false);
    }

    gameView.setSize(Engine::GetWindow().getSize().x, Engine::GetWindow().getSize().y);

    auto font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");

    scoreText.setFont(*font);
    scoreText.setCharacterSize(24);
    healthText.setFont(*font);
    healthText.setCharacterSize(24);

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

  if (player->get_components<PlayerPhysicsComponent>()[0]->getHealth() <= 0) {
      Engine::ChangeScene(&death);
  }
  gameView.setCenter(player->getPosition());
  scoreText.setString("Score: " + to_string(player->get_components<PlayerPhysicsComponent>()[0]->getScore()));
  healthText.setString("Health: " + to_string(player->get_components<PlayerPhysicsComponent>()[0]->getHealth()));

  scoreText.setPosition(gameView.getCenter().x - gameView.getSize().x / 2, gameView.getCenter().y - gameView.getSize().y / 2);
    healthText.setPosition(gameView.getCenter().x - gameView.getSize().x / 2, gameView.getCenter().y - gameView.getSize().y / 2 + 30);

  Engine::GetWindow().setView(gameView);
  Scene::Update(dt);
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
    Engine::GetWindow().draw(scoreText);
    Engine::GetWindow().draw(healthText);
  Scene::Render();
}
