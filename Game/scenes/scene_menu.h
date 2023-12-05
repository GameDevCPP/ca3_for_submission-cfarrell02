#pragma once

#include "engine.h"
#include "../components/cmp_button.h"


class MenuScene : public Scene {
protected:
    std::shared_ptr<Entity> _startButton;
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
};
