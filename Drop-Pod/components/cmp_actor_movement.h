// Header file that handles movement of entities
#pragma once
#include <ecm.h>

class ActorMovementComponent : public Component {
protected:
	bool validMove(const sf::Vector2f&);
	float _speed;
	bool _moving;
	bool _direction;

public:
	void update(double dt) override;
	void move(const sf::Vector2f&);
	void enemyMove(const sf::Vector2f&);
	void move(float x, float y);
	void render() override {}
	explicit ActorMovementComponent(Entity* p);
	ActorMovementComponent() = delete;

	float getSpeed() const;
	void setSpeed(float speed);

	bool getMoving() const;
	void setMoving(bool moving);

	bool getDirection();
	void setDirection(bool direction);
};