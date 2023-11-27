// C++ file for creating enemies
#include "cmp_monster.h"

#include "cmp_player.h"
#include "../scenes/scene_planet_level.h"
#include "cmp_sprite.h"
#include "engine.h"

using namespace std;
using namespace sf;

//Constructor
MonsterComponent::MonsterComponent(Entity* p, shared_ptr<Entity> player) : ActorMovementComponent(p),
_health(100), _player(player), _damage(2), _deathTime(0.f) { }

void MonsterComponent::set_health(int health)
{
	_health = health;
}

int MonsterComponent::get_health()
{
	return _health;
}

void MonsterComponent::set_damage(int damage)
{
	_damage = damage;
}

void MonsterComponent::attack(double dt)
{
	_attackTime -= dt;

	if (_attackTime <= 0) {
		auto playerHealth = _player->GetCompatibleComponent<PlayerComponent>()[0]->getHealth();
		_player->GetCompatibleComponent<PlayerComponent>()[0]->setHealth(playerHealth - _damage);
		_attackTime = 0.5f;

		// Sound effect
	}
}

void MonsterComponent::update(double dt)
{
	//Runs process if monster is dead
	if (_health <= 0)
	{
		_deathTime += dt;

		if (_parent->GetCompatibleComponent<AnimationComponent>()[0]->getRow() != 2)
		{
			_parent->GetCompatibleComponent<AnimationComponent>()[0]->setRow(2);
			_parent->GetCompatibleComponent<AnimationComponent>()[0]->setDuration(0.1);
		}

		if (_deathTime >= 0.61)
		{
			if (_parent->GetCompatibleComponent<AnimationComponent>()[0]->getRow() != 3)
			{
				_parent->GetCompatibleComponent<AnimationComponent>()[0]->setRow(3);
			}
		}

		if (_deathTime >= 1.1)
		{
			_parent->setAlive(false);
			_parent->setVisible(false);
			_parent->setPosition(Vector2f(-100, -100));
		}
	}
	else
	{
		auto& playerPos = _player->getPosition();
		auto& enemyPos = _parent->getPosition();
		float xDistance = playerPos.x - enemyPos.x;
		float yDistance = playerPos.y - enemyPos.y;

		auto distance = (xDistance * xDistance) + (yDistance * yDistance);

		if (distance > 3500)
		{
			_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->setMoving(true);
			Vector2f direction = normalize(playerPos - enemyPos);
			auto speed = _parent->GetCompatibleComponent<ActorMovementComponent>()[0]->getSpeed();

			auto move = Vector2f(direction.x * speed * dt, direction.y * speed * dt);

			auto flocking = Engine::flocking(_parent, move);

			if (flocking == Vector2f(0, 0))
			{
				_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->enemyMove(move);
			}
			else
			{
				_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->enemyMove(flocking);
			}

			_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->enemyMove(move);
		}
		else
		{
			_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->setMoving(false);
		}

		// Swap the animation for moving and attacking when the sprite stops moving (Reaches the player)
		if (!_parent->GetCompatibleComponent<ActorMovementComponent>()[0]->getMoving())
		{
			if (_parent->GetCompatibleComponent<AnimationComponent>()[0]->getRow() != 4)
			{
				_parent->GetCompatibleComponent<AnimationComponent>()[0]->setRow(4);
				_parent->GetCompatibleComponent<AnimationComponent>()[0]->setDuration(0.17);
			}

			// Attack
			attack(dt);
		}
		else
		{
			if (_parent->GetCompatibleComponent<AnimationComponent>()[0]->getRow() != 0)
			{
				_parent->GetCompatibleComponent<AnimationComponent>()[0]->setRow(0);
				_parent->GetCompatibleComponent<AnimationComponent>()[0]->setDuration(0.1);
			}
		}

		// Flip the sprite if moving from left.
		if (enemyPos.x < playerPos.x)
		{
			auto& p = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
			_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(-2.f, 2.f);
		}
		else
		{
			auto& p = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
			_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(2.f, 2.f);
		}
	}
}

void MonsterComponent::render() { }