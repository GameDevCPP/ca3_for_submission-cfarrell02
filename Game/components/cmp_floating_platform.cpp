//
// Created by cianf on 04/12/2023.
//

#include "cmp_floating_platform.h"
#include "cmp_physics.h"

#include <utility>

FloatingPlatformComponent::FloatingPlatformComponent(Entity *p, sf::Vector2f startPos, sf::Vector2f endPos,
                                                     float speed): Component(p), _startPos(startPos), _endPos(endPos),
                                                                     _speed(speed) {
    _parent->setPosition(_startPos);
}

void FloatingPlatformComponent::update(double dt) {
    _time += dt;
    auto dist = _endPos - _startPos;
    auto dir = dist / std::sqrt(dist.x * dist.x + dist.y * dist.y);
    auto phys = _parent->get_components<PhysicsComponent>().at(0);
    if(phys == nullptr) throw std::runtime_error("FloatingPlatformComponent::update() - no physics component found");

    if (_time > 2 * M_PI) {
        _time -= 2 * M_PI;
        std::swap(_startPos, _endPos); // Swap start and end positions
        dir = -dir; // Reverse direction
    }

    phys->setVelocity(dir * _speed);
}



void FloatingPlatformComponent::render() {
    //_parent->render();
}

