#pragma once

#include "engine.h"
#include "SFML/Graphics/Text.hpp"

class Level2Scene : public Scene {
public:

    // Views
    sf::View gameView;
    sf::View hudView;
    std::shared_ptr<Entity> player;
    std::shared_ptr<Entity> flag;

    // Text
    sf::Text scoreText;
    sf::Text healthText;
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;


};
