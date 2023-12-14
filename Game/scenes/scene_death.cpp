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
        txt->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2.0f - 75,
                                  Engine::GetWindow().getSize().y / 2.0f - 100));
        auto s = txt->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(320.0f, 100.0f));
        s->getShape().setOrigin(100.0f, 25.0f);
        auto t = txt->addComponent<TextComponent>(" Dead");
        //t->SetPosition(Vector2f(100.0f, 100.0f));
        t->SetCharacterSize(50U);
        t->SetColor(Color::Red);

        _startButton = makeEntity();
        //Button::Button(Entity* p, sf::Vector2f position, std::string text, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor)
        auto button = _startButton->addComponent<Button>(Vector2f(Engine::GetWindow().getSize().x / 2.0f - 20,
                                                                  Engine::GetWindow().getSize().y  - 200.0f),
                                                         "Restart?", Color::White, Color::Red, Color::Blue);

    }

    Engine::GetWindow().setView(Engine::GetWindow().getDefaultView());

    const int spriteSize = 128;
    const int width = Engine::GetWindow().getSize().x;
    const int height = Engine::GetWindow().getSize().y;

    for(int x = 0; x < width; x+=spriteSize) {
        for(int y = 0; y < height;  y+=spriteSize) {
            //Manually make a sprite
            sf::Sprite sprite = sf::Sprite();
            sprite.setTexture(*Resources::get<Texture>("Free/Background/Gray.png"));
            sprite.setScale(2.0f, 2.0f);
            sprite.setPosition(Vector2f(x, y));
            backgroundSprites.push_back(sprite);
        }
    }

    setLoaded(true);
}

void DeathScene::Update(const double& dt) {
    auto startButtonComponent = _startButton->get_components<Button>()[0];

    if (startButtonComponent->isPressed()) {
        cout<< startButtonComponent->buttonState << endl;
        Engine::ChangeScene(&menu);
    }



    Scene::Update(dt);
}

void DeathScene::UnLoad() {
    Scene::UnLoad();
}

void DeathScene::Render() {
    for (auto& b : backgroundSprites) {
        Engine::GetWindow().draw(b);
    }

    Scene::Render();
}
