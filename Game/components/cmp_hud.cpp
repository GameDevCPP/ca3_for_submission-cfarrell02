//
// Created by Cian Farrell on 06/12/2023.
//

#include "cmp_hud.h"
#include <system_renderer.h>


void HudComponent::update(double dt) {
    //Update text
    _healthText->setString("Health: " + std::to_string(_player->getHealth()));
    //TODO add score here

}

void HudComponent::render() {
    //Render text
    Renderer::queue(_healthText.get());
    Renderer::queue(_scoreText.get());

}

HudComponent::HudComponent(Entity *p, std::shared_ptr<PlayerPhysicsComponent> player)
        : Component(p), _player(std::move(player)) {
    _font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
    _healthText = make_shared<sf::Text>();
    _healthText->setFont(*_font);
    _healthText->setCharacterSize(20);
    _healthText->setFillColor(sf::Color::White);
    _healthText->setPosition(sf::Vector2f(0, 0));
    _healthText->setString("Health: " + std::to_string(_player->getHealth()));

    _scoreText = make_shared<sf::Text>();
    _scoreText->setFont(*_font);
    _scoreText->setCharacterSize(20);
    _scoreText->setFillColor(sf::Color::White);
    _scoreText->setPosition(sf::Vector2f(0, 20));
    _scoreText->setString("Score: 0");
}