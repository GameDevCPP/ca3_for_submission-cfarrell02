#pragma once

#include "engine.h"
#include "../components/cmp_button.h"


class MenuScene : public Scene {
protected:
    std::shared_ptr<Entity> _startButton;
    std::shared_ptr<sf::SoundBuffer> _menuMusic;
    sf::Sound _sound;

public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

    void UnLoad() override;

};
