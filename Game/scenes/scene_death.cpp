#include "scene_death.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void DeathScene::Load() {
    cout << "Death Load \n";
    {
        auto txt = makeEntity();
        txt->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2.0f,
                                  Engine::GetWindow().getSize().y / 2.0f));
        auto s = txt->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(320.0f, 100.0f));
        s->getShape().setOrigin(100.0f, 25.0f);
        auto t = txt->addComponent<TextComponent>("Dead");
        //t->SetPosition(Vector2f(100.0f, 100.0f));
        t->SetCharacterSize(50U);
        t->SetColor(Color::Red);

        _startButton = makeEntity();
        //Button::Button(Entity* p, sf::Vector2f position, std::string text, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor)
        auto button = _startButton->addComponent<Button>(Vector2f(Engine::GetWindow().getSize().x / 2.0f,
                                                                  Engine::GetWindow().getSize().y / 2.0f + 100.0f),
                                                         "Restart?", Color::White, Color::Red, Color::Blue);

    }
    setLoaded(true);
}

void DeathScene::Update(const double& dt) {
    // cout << "Menu Update "<<dt<<"\n";
    auto startButtonComponent = _startButton->get_components<Button>()[0];
    if (startButtonComponent->isPressed()) {
        cout << "Start pressed\n";
        Engine::ChangeScene(&level1);
    }

    Scene::Update(dt);
}

void DeathScene::UnLoad() {
    Scene::UnLoad();
}

void DeathScene::Render() {
    Scene::Render();
}
