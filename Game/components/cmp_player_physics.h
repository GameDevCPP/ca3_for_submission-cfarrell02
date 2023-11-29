#pragma once

#include "cmp_physics.h"

enum class PlayerAnimationState { IDLE, WALKING, JUMPING };

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;
  PlayerAnimationState _animationState = PlayerAnimationState::IDLE;
  void SetAnimation(std::string textureUrl, int frames, float frameTime,Entity* player);

  bool isGrounded() const;

public:
  void update(double dt) override;

  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

  PlayerPhysicsComponent() = delete;
};
