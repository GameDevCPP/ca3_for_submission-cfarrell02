//
// Created by cianf on 04/12/2023.
//

#ifndef DROP_POD_CMP_PARTICLE_GENERATOR_H
#define DROP_POD_CMP_PARTICLE_GENERATOR_H

#include <SFML/Graphics.hpp>
#include "ecm.h"

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    std::shared_ptr<Entity> entity;
    float timer = 0.f;
    float life;
};


class ParticleGenerator: public Component {
protected:
    int _amount;
    float _life;
    float _speed;
    float _spread;
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    std::shared_ptr<sf::Texture> _texture;
    std::vector<Particle> _particles;
    std::shared_ptr<Entity> _player;
    bool _active = true;
    float _rate;
    bool _loop;
    float _timer = 0.f;
    void regenerateParticle();

public:
    ParticleGenerator(Entity* p, int amount, float life, float speed, float spread, sf::Vector2f position,
                      std::shared_ptr<sf::Texture> texture, std::shared_ptr<Entity> player, float rate = -1, bool loop = false);
    ParticleGenerator() = delete;
    void update(double dt) override;
    void render() override;
    void setTexture(std::shared_ptr<sf::Texture> texture);
    void setAmount(int amount);
    void setLife(float life);
    void setSpeed(float speed);
    void setSpread(float spread);
    void setPosition(sf::Vector2f position);
    void setVelocity(sf::Vector2f velocity);
    sf::Sprite getParticleSprite();
    int getAmount();
    float getLife();
    float getSpeed();
    float getSpread();
    sf::Vector2f getPosition();
    sf::Vector2f getVelocity();
    void activate(bool active);
    bool isActive();

};


#endif //DROP_POD_CMP_PARTICLE_GENERATOR_H
