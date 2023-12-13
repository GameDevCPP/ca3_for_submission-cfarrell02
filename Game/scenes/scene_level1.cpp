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

        playerHealth = 100;
        playerScore = 0;




    auto walls = ls::findTiles(ls::WALL);
    auto floors = ls::findTiles(ls::FLOOR);
    auto ceilings = ls::findTiles(ls::CEILING);
    walls.insert(walls.end(), floors.begin(), floors.end());
    walls.insert(walls.end(), ceilings.begin(), ceilings.end());
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(20.f, 20.f); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(b2_staticBody, Vector2f(40.f, 40.f));
    }
        auto particles = ls::findTiles(ls::PARTICLEGENERATOR);
    for (auto p : particles) {
        auto particle = makeEntity();
        auto pos = ls::getTilePosition(p);
        //Entity *p, int amount, float life, float speed, float spread,
        particle->addComponent<ParticleGenerator>(5, 2, 10.f, 20.f, pos,
                                                  Resources::get<Texture>("Free/Traps/Sand Mud Ice/Ice Particle.png"),
                                                  player, .1, true);


    }
    }

    //Add in floating platform
    {
        auto platforms = ls::findTiles(ls::WAYPOINT);
        for(int i = 0; i < platforms.size() -1; i+=2) {
            auto startPos = ls::getTilePosition(platforms[i]);
            auto endPos = ls::getTilePosition(platforms[i+1]);
            auto platform = makeEntity();
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
        auto hazardLocations = ls::findTiles(ls::HAZARD);
        for(auto p : hazardLocations) {
            auto pos = ls::getTilePosition(p);
            auto hazard = makeEntity();
//        pos = {pos.x + 40, pos.y + 40};
            hazard->setPosition(pos);
            auto hazardTexture = Resources::get<Texture>("Free/Traps/Spikes/Idle.png");
            auto hsprite = hazard->addComponent<SpriteComponent>();
            hsprite->setTexture(hazardTexture);
            //hsprite->getSprite().setOrigin(16.f, 16.f);
            hsprite->getSprite().setScale(2.5f, 2.5f);
            auto hHazard = hazard->addComponent<HazardComponent>(10, player);
        }

    }
        auto pickupLocations = ls::findTiles(ls::SCORE);
    if (!pickupLocations.empty()) {
        for (auto p: pickupLocations) {
            auto pos = ls::getTilePosition(p);
            pos = {pos.x + 20, pos.y + 30};
            auto pickup = makeEntity();
            pickup->setPosition(pos);
            cout << "Pickup at: " << pos.x << ", " << pos.y << endl;
            auto pickupTexture = Resources::get<Texture>("Free/Items/Fruits/Apple.png");
            cout << "Pickup Texture: " << pickupTexture->getSize().x << ", " << pickupTexture->getSize().y << endl;
            auto psprite = pickup->addComponent<SpriteComponent>();
            psprite->setTexture(pickupTexture);
            psprite->getSprite().setOrigin(16.f, 16.f);
            auto animComp = pickup->addComponent<AnimationComponent>();
            animComp->setAnimation(17, .1, pickupTexture, IntRect(Vector2i(0, 0), Vector2i(32, 32)));
            pickup->addComponent<PickupComponent>(PickupComponent::PickupType::SCORE, 10, player);
        }
    }


        auto pickup2Locations = ls::findTiles(ls::HEALTH);
    if (!pickupLocations.empty()) {
        for(auto p : pickup2Locations){
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



    {
        flag = makeEntity();
        auto pos = ls::getTilePosition(ls::findTiles(ls::ENDLEVEL)[0]);
        pos.y += 10; // Offset the flad to be on the ground
        flag->setPosition(pos);
        auto endTexture = Resources::get<Texture>("Free/Items/Checkpoints/Checkpoint/Checkpoint (Flag Idle)(64x64).png");
        auto esprite = flag->addComponent<SpriteComponent>();
        esprite->setTexture(endTexture);
        esprite->getSprite().setOrigin(32.f, 32.f);
        auto animComp = flag->addComponent<AnimationComponent>();
        animComp->setAnimation(10, .1, endTexture, IntRect(Vector2i(0, 0), Vector2i(64, 64)));
        flag->addComponent<NextLevelComponent >(player,std::make_shared<DeathScene>(), false);
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
  else if(flag->get_components<NextLevelComponent>()[0]->getIsAtFlag()){
      Engine::ChangeScene(&level2);
  }
if (player != nullptr) {
    gameView.setCenter(player->getPosition());
    scoreText.setString("Score: " + to_string(player->get_components<PlayerPhysicsComponent>()[0]->getScore()));
    healthText.setString("Health: " + to_string(player->get_components<PlayerPhysicsComponent>()[0]->getHealth()));
    playerHealth = player->get_components<PlayerPhysicsComponent>()[0]->getHealth();
    playerScore = player->get_components<PlayerPhysicsComponent>()[0]->getScore();
}

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
