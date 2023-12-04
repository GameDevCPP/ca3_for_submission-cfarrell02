//
// Created by cianf on 04/12/2023.
//

#ifndef DROP_POD_CMP_FLOATING_PLATFORM_H
#define DROP_POD_CMP_FLOATING_PLATFORM_H
#include <SFML/Graphics.hpp>
#include "engine.h"
#include "../components/cmp_sprite.h"


class FloatingPlatformComponent : public Component {
protected:
    sf::Vector2f _startPos;
    sf::Vector2f _endPos;
    float _speed;
    float _time = 0;

public:
    void update(double dt) override;
    void render() override ;
    explicit FloatingPlatformComponent(Entity* p, sf::Vector2f startPos, sf::Vector2f endPos, float speed);
    FloatingPlatformComponent() = delete;
//    std::shared_ptr<Entity> getPlatform() {return _platform;}
//    void setPlatform(std::shared_ptr<Entity> platform) {_platform = platform;}

};


#endif //DROP_POD_CMP_FLOATING_PLATFORM_H
