#include "cmp_player_physics.h"
#include "system_physics.h"
#include "engine.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include "../game.h"

using namespace std;
using namespace sf;
using namespace Physics;

bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::SetAnimation( PlayerAnimationState state, int frames, float frameTime, Entity* player){
//    cout<<"Setting Animation: "<<textureUrl<<endl;
    auto idleSprite = _animationTextures[state];
    //player->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setTexture(*idleSprite);
    player->GetCompatibleComponent<AnimationComponent>()[0]->setAnimation(frames, frameTime, idleSprite, IntRect(Vector2i(0, 0), Vector2i(32, 32)));
}

void PlayerPhysicsComponent::update(double dt) {

  const auto pos = _parent->getPosition();

  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }

  if (Keyboard::isKeyPressed(Keyboard::A) ||
      Keyboard::isKeyPressed(Keyboard::D)) {
    // Moving Either Left or Right
    if (Keyboard::isKeyPressed(Keyboard::D)) {
      if (getVelocity().x < _maxVelocity.x) {
          impulse({(float) (dt * _groundspeed), 0});
      }
    } else {
      if (getVelocity().x > -_maxVelocity.x) {
          impulse({-(float) (dt * _groundspeed), 0});
      }
    }
  } else {
    // Dampen X axis movement
    dampen({0.9f, 1.0f});
  }

  // Handle Jump
  if (Keyboard::isKeyPressed(Keyboard::Space)) {
    _grounded = isGrounded();
    if (_grounded) {
      setVelocity(Vector2f(getVelocity().x, 0.f));
      teleport(Vector2f(pos.x, pos.y - 2.0f));
      impulse(Vector2f(0, -6.f));
    }
  }

  //Are we in air?
  if (!_grounded) {
    // Check to see if we have landed yet
    _grounded = isGrounded();
    // disable friction while jumping
    setFriction(0.f);
  } else {
    setFriction(0.2f);
  }

  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);

//  cout<<"Velocity: "<<v.x<<", "<<v.y<<endl;
  setVelocity(v);

  // Handle Animation Behaviour
    if(!_grounded){
        // Jumping Texture
        if(_animationState != PlayerAnimationState::JUMPING){
            SetAnimation(PlayerAnimationState::JUMPING, 9, 1, _parent);
            _animationState = PlayerAnimationState::JUMPING;
        }
    }
    else if(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D)) {
        // Running Texture
        if(_animationState != PlayerAnimationState::WALKING){
            SetAnimation(PlayerAnimationState::WALKING, 8, 0.05, _parent);
            _animationState = PlayerAnimationState::WALKING;
        }
    }else {
        // Back to Idle Texture
        if (_animationState != PlayerAnimationState::IDLE) {
            SetAnimation(PlayerAnimationState::IDLE, 11, 0.05, _parent);
            _animationState = PlayerAnimationState::IDLE;
        }
    }

    if (getVelocity().x > 0) {
        _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(1, 1);
    }else if (getVelocity().x < 0) {
        _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(-1, 1);
    }

  PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size, int health)
    : PhysicsComponent(p, b2_dynamicBody, size), _health(health),
      _maxHealth(health) {
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(200.f, 400.f);
  _groundspeed = 30.f;
  _grounded = false;
  _score = 0;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
    hurt_sound_buffer = Resources::get<SoundBuffer>("Hit.wav");
    pickup_sound_buffer = Resources::get<SoundBuffer>("Pickup.wav");

}

void PlayerPhysicsComponent::setHealth(int health, bool playSound) {
    if(playSound){
        if (health < _health) {
            soundClick.setVolume(1);
            soundClick.setBuffer(*hurt_sound_buffer);
            soundClick.play();
        }
        else if (health > _health) {
            soundClick.setVolume(100);
            soundClick.setBuffer(*pickup_sound_buffer);
            soundClick.play();
        }
    }

    _health = health;
    if (_health > _maxHealth) {
        _health = _maxHealth;
    }
    if (_health <= 0) {
        _parent->setForDelete();
        _health = 0;
    }

}

void PlayerPhysicsComponent::setScore(int score, bool playSound) {
    if(playSound) {
        if (score > _score) {
            soundClick.setVolume(100);
            soundClick.setBuffer(*pickup_sound_buffer);
            soundClick.play();
        }
    }
    _score = score;
    if (_score < 0) {
        _score = 0;
    }

}
