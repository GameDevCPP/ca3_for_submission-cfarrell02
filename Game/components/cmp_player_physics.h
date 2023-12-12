#pragma once

#include <map>
#include "cmp_physics.h"
#include "system_resources.h"


enum class PlayerAnimationState { IDLE, WALKING, JUMPING };


using namespace std;
class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;
  int _health;
  const int _maxHealth;
    int _score;

  PlayerAnimationState _animationState = PlayerAnimationState::IDLE;
    std::map<PlayerAnimationState, std::shared_ptr<sf::Texture>> _animationTextures =
            {
                    {PlayerAnimationState::IDLE, Resources::get<sf::Texture>("Free/Main Characters/Ninja Frog/Idle (32x32).png")},
                    {PlayerAnimationState::WALKING, Resources::get<sf::Texture>("Free/Main Characters/Ninja Frog/Run (32x32).png")},
                    {PlayerAnimationState::JUMPING, Resources::get<sf::Texture>("Free/Main Characters/Ninja Frog/Jump (32x32) 2.png")}
            };

  void SetAnimation(PlayerAnimationState state, int frames, float frameTime,Entity* player);


  bool isGrounded() const;

public:
  void update(double dt) override;

  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, int health = 100);

  PlayerPhysicsComponent() = delete;

  int getHealth() const { return _health; }
    void setHealth(int health);
    int getScore() const { return _score; }
    void setScore(int score);
};
