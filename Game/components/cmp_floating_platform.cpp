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
    auto dist = _endPos - _startPos;
    auto dir = dist / std::sqrt(dist.x * dist.x + dist.y);
    auto phys = _parent->get_components<PhysicsComponent>().at(0);
    if(phys == nullptr) throw std::runtime_error("FloatingPlatformComponent::update() - no physics component found");

    auto pos = _parent->getPosition();
    float threshold = 5.0f; // Adjust the threshold distance as needed

    if (_movingToEnd) {
            dir = {-abs(dir.x), dir.y}; // Reverse direction
           // std::cout<< "Moving to start" << ", " << dir.x << ", " << dir.y << std::endl;
    } else {
            //std::cout<< "Moving to end" << ", " << dir.x << ", " << dir.y << std::endl;
            dir = {abs(dir.x), dir.y}; // Reverse direction=
    }

    if (pos.x > _endPos.x - threshold && pos.x < _endPos.x + threshold) {
        _movingToEnd = true;
    } else if (pos.x > _startPos.x - threshold && pos.x < _startPos.x + threshold) {
        _movingToEnd = false;
    }
    phys->setVelocity(dir * _speed);
}





void FloatingPlatformComponent::render() {
    //_parent->render();
}

