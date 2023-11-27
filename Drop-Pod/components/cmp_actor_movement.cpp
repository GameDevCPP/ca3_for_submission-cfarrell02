//C++ file that handles movement of entities
#include "cmp_actor_movement.h"
#include <LevelSystem.h>
#include <engine.h>
using namespace sf;
using namespace std;

void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
	: Component(p), _speed(80.0f), _moving(false), _direction(false) {}

bool ActorMovementComponent::validMove(const sf::Vector2f& pos) {
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
	// return true;
}
//Moves the player
void ActorMovementComponent::move(const sf::Vector2f& p) {
	auto pp = _parent->getPosition() + p;
	if (validMove(pp)) {
		_parent->setPosition(pp);
	}
}
//Moves the enemy
void ActorMovementComponent::enemyMove(const sf::Vector2f& p) {
	auto pp = _parent->getPosition() + p;
	_parent->setPosition(pp);
}
//Takes an x and y value and puts it in a Vector2f to be used in the other move function
void ActorMovementComponent::move(float x, float y) {
	move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const { return _speed; }

void ActorMovementComponent::setSpeed(float speed) { _speed = speed; }

bool ActorMovementComponent::getMoving() const { return _moving; }

void ActorMovementComponent::setMoving(bool moving) { _moving = moving; }

bool ActorMovementComponent::getDirection() { return _direction; }

void ActorMovementComponent::setDirection(bool direction) { _direction = direction; }