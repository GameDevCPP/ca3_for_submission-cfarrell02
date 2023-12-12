//
// Created by cianf on 12/12/2023.
//

#include "cmp_pickup.h"
#include "cmp_sprite.h"
#include "cmp_player_physics.h"

#include <utility>

void PickupComponent::update(double dt) {
    auto playerBounds = _player->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds();
    if (playerBounds.contains(_parent->getPosition())) {

        if (_type == PickupType::HEALTH) {
            _player->get_components<PlayerPhysicsComponent>()[0]->setHealth(_player->get_components<PlayerPhysicsComponent>()[0]->getHealth() + _value);
        } else if (_type == PickupType::SCORE) {
            _player->get_components<PlayerPhysicsComponent>()[0]->setScore(_player->get_components<PlayerPhysicsComponent>()[0]->getScore() + _value);
        }

        _parent->setForDelete();
    }
}

void PickupComponent::render() {

}

PickupComponent::PickupComponent(Entity *p, PickupComponent::PickupType type, int value,
                                 std::shared_ptr<Entity> player) : Component(p), _type(type), _value(value), _player(std::move(player)) {

}
