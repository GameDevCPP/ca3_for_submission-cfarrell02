//
// Created by cianf on 05/12/2023.
//

#include "cmp_hazard.h"

#include <utility>
#include "../engine/engine.h"
#include "../lib_ecm/ecm.h"
#include "./cmp_player_physics.h"

void HazardComponent::update(double dt) {
    _timer += dt;

    if (_timer > _rate) {
        _timer = 0.0f;
    }
    else {
        return;
    }

    auto playerPos = _player-> getPosition();
    playerPos.x += 32;
    playerPos.y += 32;

    auto hazardSpriteBounds = _parent->get_components<SpriteComponent>().at(0)->getSprite().getLocalBounds();
    auto pos = _parent->getPosition();
    hazardSpriteBounds.left = pos.x;
    hazardSpriteBounds.top = pos.y;
//    cout<<"Player Pos: "<<playerPos.x<<", "<<playerPos.y<<endl;
//    cout<<"Hazard Pos: "<<hazardSpriteBounds.left<<" + "<<hazardSpriteBounds.left + hazardSpriteBounds.width<<", "<<hazardSpriteBounds.top<<" + "<<hazardSpriteBounds.top + hazardSpriteBounds.height<<endl;
    if (hazardSpriteBounds.contains(playerPos.x, playerPos.y)) {
            //Player is in contact with hazard
            std::cout << "_player is in contact with hazard" << std::endl;
            auto playerPhysics = _player->get_components<PlayerPhysicsComponent>().at(0);
            if(playerPhysics == nullptr) throw std::runtime_error("HazardComponent::update() - no player physics component found");
            int playerHealth = _player->get_components<PlayerPhysicsComponent>().at(0)->getHealth();
            _player->get_components<PlayerPhysicsComponent>().at(0)->setHealth(playerHealth - _damage);
    }

}

void HazardComponent::render() {

}

HazardComponent::HazardComponent(Entity *p, int damage,  std::shared_ptr<Entity> player, float rate)
: Component(p), _damage(damage), _player(std::move(player)), _rate(rate) {

}
