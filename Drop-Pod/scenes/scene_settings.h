#pragma once

#include "engine.h"

class SettingsScene : public Scene {
public:
	SettingsScene() = default;
	~SettingsScene() override = default;
	void Load() override;

	void Update(const double& dt) override;
	std::shared_ptr<Entity> btnArrows;
	std::shared_ptr<Entity> btnWsad;
	std::shared_ptr<Entity> btn1280;
	std::shared_ptr<Entity> btn1920;
	std::shared_ptr<Entity> btnFrame;
	std::shared_ptr<Entity> btnSoundUp;
	std::shared_ptr<Entity> btnSoundDown;
	std::shared_ptr<Entity> btnBack;
};
