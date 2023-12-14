#include "scene_level2.h"
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


void Level2Scene::Load() {
    cout << " Scene 1 Load" << endl;
    ls::loadTextureFile("res/img/Free/Terrain/Terrain (16x16).png", 16);
    ls::loadLevelFile("res/level2.json", 40.0f);

    _levelMusic = Resources::get<sf::SoundBuffer>("Background Music2.wav");
    _sound.setBuffer(*_levelMusic);
    _sound.play();
    _sound.setLoop(true);
    _sound.setVolume(25);

  // Create player
    {
        player = makeEntity();
        auto startPos = ls::getTilePosition(ls::findTiles(ls::START)[0]);

        player->setPosition(startPos);
        auto idleTexture = Resources::get<Texture>("Free/Main Characters/Mask Dude/Idle (32x32).png");
        auto psprite = player->addComponent<SpriteComponent>();
      //  psprite->setTexture(idleTexture);
        psprite->getSprite().setOrigin(16.f, 16.f);

        auto animComp = player->addComponent<AnimationComponent>();
        auto pPhys = player->addComponent<PlayerPhysicsComponent>(Vector2f(16.f, 30));
        pPhys->setHealth(playerHealth, false);
        pPhys->setScore(playerScore, false);






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
    // First particle generator
        auto particles = ls::findTiles(ls::PARTICLEGENERATOR);
        int spread[] = {100,100,200};
    for (int i = 0; i < particles.size(); i++) {
        auto p = particles[i];
        auto particle = makeEntity();
        auto pos = ls::getTilePosition(p);
        pos.x += 12;
        //Entity *p, int amount, float life, float speed, float spread,
        particle->addComponent<ParticleGenerator>(10, 2, 10.f, spread[i], pos,
                                                  Resources::get<Texture>("Free/Traps/Sand Mud Ice/Ice Particle.png"),
                                                  player, .1, true);
    }

    }

    //Add in floating platform
    auto platforms = ls::findTiles(ls::WAYPOINT);
    if (platforms.size() % 2 == 0 && !platforms.empty()) {
        generatePlatforms(platforms, 3, 0);
        generatePlatforms(platforms, 3, 2);
        generatePlatforms(platforms, 4, 4);
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

    const int spriteSize = 128;
    const int width = Engine::GetWindow().getSize().x;
    const int height = Engine::GetWindow().getSize().y;

    for(int x = 0; x < width; x+=spriteSize) {
        for(int y = 0; y < height;  y+=spriteSize) {
            //Manually make a sprite
            sf::Sprite sprite = sf::Sprite();
            sprite.setTexture(*Resources::get<Texture>("Free/Background/Yellow.png"));
            sprite.setScale(2.0f, 2.0f);
            sprite.setPosition(Vector2f(x, y));
            backgroundSprites.push_back(sprite);
        }
    }


    gameView.setSize(Engine::GetWindow().getSize().x, Engine::GetWindow().getSize().y);

    auto font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");

    scoreText.setFont(*font);
    scoreText.setCharacterSize(24);
    healthText.setFont(*font);
    healthText.setCharacterSize(24);

  cout << " Scene 2 Load Done" << endl;

  setLoaded(true);
}

void Level2Scene::generatePlatforms(std::vector<sf::Vector2ul> &platforms, int amount, int startIndex) {
    for (int i = 0; i < amount; i++) {
        auto startPos = ls::getTilePosition(platforms[startIndex]);
        startPos.x += i*40;
        auto endPos = ls::getTilePosition(platforms[startIndex+1]);
        endPos.x += i*40;
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

void Level2Scene::UnLoad() {
  cout << "Scene 2 Unload" << endl;
    _sound.stop();

    player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {

  if (player->get_components<PlayerPhysicsComponent>()[0]->getHealth() <= 0) {
      Engine::ChangeScene(&death);
  }
  else if(flag->get_components<NextLevelComponent>()[0]->getIsAtFlag()){
      Engine::ChangeScene(&level3);
  }

    if(player != nullptr) {
        gameView.setCenter(player->getPosition());
        scoreText.setString("Score: " + to_string(player->get_components<PlayerPhysicsComponent>()[0]->getScore()));
        healthText.setString("Health: " + to_string(player->get_components<PlayerPhysicsComponent>()[0]->getHealth()));
        playerHealth = player->get_components<PlayerPhysicsComponent>()[0]->getHealth();
        playerScore = player->get_components<PlayerPhysicsComponent>()[0]->getScore();

        const int spriteSize = 128;
        const int width = Engine::GetWindow().getSize().x;
        const int height = Engine::GetWindow().getSize().y;

        for (int x = 0; x < width; x += spriteSize) {
            for (int y = 0; y < height; y += spriteSize) {
                // Calculate the index correctly for sprite retrieval
                int index = x / spriteSize + y / spriteSize * (width / spriteSize);
                auto& sprite = backgroundSprites[index];
                auto newPos = Vector2f(gameView.getCenter().x - gameView.getSize().x / 2 + x,
                                       gameView.getCenter().y - gameView.getSize().y / 2 + y);
                sprite.setPosition(newPos);
            }
        }


}

  scoreText.setPosition(gameView.getCenter().x - gameView.getSize().x / 2, gameView.getCenter().y - gameView.getSize().y / 2);
    healthText.setPosition(gameView.getCenter().x - gameView.getSize().x / 2, gameView.getCenter().y - gameView.getSize().y / 2 + 30);

  Engine::GetWindow().setView(gameView);
  Scene::Update(dt);
}

void Level2Scene::Render() {
    for(auto &s : backgroundSprites) {
        Engine::GetWindow().draw(s);
    }
  ls::render(Engine::GetWindow());
    Engine::GetWindow().draw(scoreText);
    Engine::GetWindow().draw(healthText);
  Scene::Render();
}
