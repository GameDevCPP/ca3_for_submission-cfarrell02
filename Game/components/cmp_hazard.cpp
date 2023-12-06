//
// Created by cianf on 05/12/2023.
//

#include "cmp_hazard.h"
#include "../engine/engine.h"
#include "../lib_ecm/ecm.h"
#include "./cmp_player_physics.h"

void HazardComponent::update(double dt) {
    //Check if player is in contact with hazard
    auto scene = _parent->scene;
    auto player = scene->getEcm().find("player").at(0);

    if(player == nullptr) throw std::runtime_error("HazardComponent::update() - no player found");

    auto playerPos = player-> getPosition();

    auto pos = _parent->getPosition();

    float threshold = 5.0f; // Adjust the threshold distance as needed

    if (pos.x > playerPos.x - threshold && pos.x < playerPos.x + threshold) {
        if (pos.y > playerPos.y - threshold && pos.y < playerPos.y + threshold) {
            //Player is in contact with hazard
            std::cout << "Player is in contact with hazard" << std::endl;
            auto playerPhysics = player->get_components<PlayerPhysicsComponent>().at(0);
            if(playerPhysics == nullptr) throw std::runtime_error("HazardComponent::update() - no player physics component found");
            int playerHealth = player->get_components<PlayerPhysicsComponent>().at(0)->getHealth();
            player->get_components<PlayerPhysicsComponent>().at(0)->setHealth(playerHealth - _damage);

        }
    }




}

void HazardComponent::render() {

}

HazardComponent::HazardComponent(Entity *p, int damage) : Component(p), _damage(damage) {

}
