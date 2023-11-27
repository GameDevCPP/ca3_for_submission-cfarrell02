// Player component C++ file
#include "cmp_player.h"
#include <SFML/Window.hpp>

#include "cmp_sprite.h"
#include "../drop_pod_game.h"
using namespace std;
using namespace sf;

int _health;

//Constructor
PlayerComponent::PlayerComponent(Entity* p) : ActorMovementComponent(p) {
	_health = 100;
}

void PlayerComponent::setHealth(int health)
{
	_health = health;
}

int PlayerComponent::getHealth()
{
	return _health;
}

void PlayerComponent::update(const double dt)
{
	if (_health <= 0)
	{
		_parent->setAlive(false);
	}

	float directX = 0.f;
	float directY = 0.f;

	//Checks which control scheme is used and checks if a movement key is being pressed
	switch (switchState) {
	case WSAD:

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			directX--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			directX++;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			directY--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			directY++;
		break;

	case Arrows:

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			directX--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			directX++;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			directY--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			directY++;
		break;
	}

	// Check if the player is moving.
	if (directX != 0 || directY != 0)
	{
		_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->setMoving(true);
	}
	else
	{
		_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->setMoving(false);
	}

	// Check the direction of the player.
	if (directX < 0)
	{
		_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->setDirection(true);
	}
	else
	{
		_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->setDirection(false);
	}

	auto speed = _parent->GetCompatibleComponent<ActorMovementComponent>()[0]->getSpeed();
	_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->move(Vector2f(directX * speed * dt, directY * speed * dt));

	// Switch between idle and moving animation for player moving.
	if (!_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->getMoving())
	{
		IntRect playerRect = { Vector2i(0, 0), Vector2i(150, 150) };
		_parent->GetCompatibleComponent<AnimationComponent>()[0]->setAnimation(8, 0.1, Resources::get<Texture>("Idle.png"), playerRect);
	}
	else
	{
		IntRect playerRect = { Vector2i(0, 0), Vector2i(150, 150) };
		_parent->GetCompatibleComponent<AnimationComponent>()[0]->setAnimation(8, 0.1, Resources::get<Texture>("Run.png"), playerRect);
	}

	// Flip the sprite if moving left.
	if (_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->getDirection())
	{
		auto& p = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
		_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(p.getLocalBounds().width * 0.5, p.getLocalBounds().height * 0.5);
		_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(-2.f, 2.f);
	}
	else
	{
		auto& p = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
		_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(p.getLocalBounds().width * 0.5, p.getLocalBounds().height * 0.5);
		_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(2.f, 2.f);
	}
}

void PlayerComponent::render() {}