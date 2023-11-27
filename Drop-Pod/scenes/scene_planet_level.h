#pragma once

#include "engine.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class PlanetLevelScene : public Scene {
public:
	EntityManager* ecm;

	// Views
	sf::View gameView;
	sf::View hudView;

	// Level
	float speed;
	int xCount;
	int yCount;
	sf::Vector2f startingCenter;
	bool viewToggle;
	bool pauseGame;
	std::string result;

	// Player
	std::shared_ptr<Entity> player;
	std::shared_ptr<sf::Texture> playerSpriteIdle;
	std::shared_ptr<sf::Texture> playerSpriteMoving;
	sf::IntRect playerRect;

	std::shared_ptr<sf::SoundBuffer> soundShoot_buffer;
	std::shared_ptr<sf::Sound> soundShoot;

	sf::RectangleShape* redBar;
	sf::RectangleShape* greenBar;
	sf::Text* healthText;

	// Enemies
	float totalTime;
	int monsterCount;
	int damage;
	float monsterSpeed;

	// Actual HUD
	float tempTime;
	int minutes;
	int seconds;
	sf::Text* timer;
	sf::Text* endText;
	sf::Text* endExitText;

	// Shooting Delay
	float fireTime;

	void init();

	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;

	void render_end() const;

	void SpawnEnemy(int damage, float speed);

	sf::Vector2f random_position() const;
};