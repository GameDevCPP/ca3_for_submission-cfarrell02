//Shooting component header file
#pragma once

#include "ecm.h"
#include "cmp_actor_movement.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Bullet : sf::Sprite {
protected:
	void _update(const double dt);
	bool isVisible;
	sf::Vector2f mousePos;
	float angle;
	int _damage;

	std::shared_ptr<sf::SoundBuffer> soundHit_buffer;
	std::shared_ptr<sf::Sound> soundHit;

public:
	static void update(const double dt);
	static void render();
	static void fire(const sf::Vector2f& pos);
	static void init();
	static void setAngle(float a, Bullet& b);

	~Bullet() = default;
	Bullet();
};

class ShootingComponent : public ActorMovementComponent {
protected:

	std::vector<Bullet> bullets;
	unsigned int bulletCount;
	float angleshot;

public:
	ShootingComponent() = delete;

	explicit ShootingComponent(Entity* p);

	void Fire();
	void update(double dt) override;
	void render() override;
};
