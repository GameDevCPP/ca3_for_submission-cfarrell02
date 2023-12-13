#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "../components/cmp_particle_generator.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void MenuScene::Load() {
  cout << "Menu Load \n";

    auto particle = makeEntity();
    auto pos = Vector2f(Engine::GetWindow().getSize().x / 2.0f,
                        -1000);
    //Entity *p, int amount, float life, float speed, float spread,
    particle->addComponent<ParticleGenerator>(10, 10, 10.f, 2000, pos,
                                              Resources::get<Texture>("Free/Traps/Sand Mud Ice/Ice Particle.png"),
                                              nullptr, .1, true);
    const int spriteSize = 128;
    const int width = Engine::GetWindow().getSize().x;
    const int height = Engine::GetWindow().getSize().y;
    for (int x = 0; x < width; x+=spriteSize) {
        for (int y = 0; y < height;  y+=spriteSize) {
            auto background = makeEntity();
            auto s = background->addComponent<SpriteComponent>();
            s->setTexture(Resources::get<Texture>("Free/Background/Blue.png"));
            s->getSprite().setScale(2.0f, 2.0f);
            background->setPosition(Vector2f(x, y));
        }
    }


    {
    auto txt = makeEntity();
    txt->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2.0f - 50,
                              Engine::GetWindow().getSize().y / 2.0f-100));
//      auto s = txt->addComponent<ShapeComponent>();
//      s->setShape<sf::RectangleShape>(Vector2f(330.0f, 100.0f));
//      s->getShape().setOrigin(100.0f, 25.0f);
    auto t = txt->addComponent<TextComponent>("Deadly Rain");
//    t->SetPosition(Vector2f(10,0));
    t->SetCharacterSize(50U);
    t->SetColor(Color::Blue);
    }

    {


    _startButton = makeEntity();
    //Button::Button(Entity* p, sf::Vector2f position, std::string text, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor)
    auto button = _startButton->addComponent<Button>(Vector2f(Engine::GetWindow().getSize().x / 2.0f,
                                                     Engine::GetWindow().getSize().y / 2.0f + 100.0f),
                                           "Start", Color::White, Color::Red, Color::Blue);

    _menuMusic = Resources::get<sf::SoundBuffer>("Menu Music.wav");
    _sound.setBuffer(*_menuMusic);
    _sound.play();
    _sound.setLoop(true);
    _sound.setVolume(50);



    }

    Engine::GetWindow().setView(Engine::GetWindow().getDefaultView());

    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";
    auto startButtonComponent = _startButton->get_components<Button>()[0];
  if (startButtonComponent->isPressed()) {
    cout << "Start pressed\n";
    Engine::ChangeScene(&level1);
  }

  Scene::Update(dt);
}

void MenuScene::UnLoad() {
    cout << "Menu Unload" << endl;
    _sound.stop();
    Scene::UnLoad();
}
