#pragma once

#include "engine.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Level2Scene : public Scene {
public:

    // Views
    sf::View gameView;
    sf::View hudView;
    std::shared_ptr<Entity> player;
    std::shared_ptr<Entity> flag;
    std::vector<sf::Sprite> backgroundSprites;

    // Text
    sf::Text scoreText;
    sf::Text healthText;
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;


    void generatePlatforms(std::vector<sf::Vector2ul> &platforms, int amount, int startIndex);

protected:
    std::shared_ptr<sf::SoundBuffer> _levelMusic;
    sf::Sound _sound;
};
