#pragma once

#include "engine.h"
#include "SFML/Graphics/Text.hpp"

class Level1Scene : public Scene {
public:

    // Views
    sf::View gameView;
    sf::View hudView;
    std::shared_ptr<Entity> player;

    // Text
    sf::Text scoreText;
    sf::Text healthText;
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};
