#include "cmp_particle_generator.h"
#include "engine.h"
#include "cmp_sprite.h"
#include "cmp_physics.h"
#include "cmp_player_physics.h"
#include <iostream>
#include <utility>

ParticleGenerator::ParticleGenerator(Entity *p, int amount, float life, float speed, float spread,
                                     sf::Vector2f position, std::shared_ptr<sf::Texture> texture,std::shared_ptr<Entity> player,
                                     float rate, bool loop)
        : Component(p),
          _amount(amount),
          _life(life),
          _speed(speed),
          _spread(spread),
          _position(position),
          _texture(std::move(texture)),
          _loop(loop),
          _active(true),
          _player(std::move(player)),
          _rate(rate){

    for (int i = 0; i < _amount; ++i) {
        regenerateParticle();
    }
}

void ParticleGenerator::regenerateParticle() {

        auto particle = _parent->scene->makeEntity();
        // Vary position by spread
        auto spread = sf::Vector2f((rand() % (int) _spread) - _spread / 2, (rand() % (int) _spread) - _spread / 2);
        particle->setPosition(_position + spread);
        particle->addComponent<PhysicsComponent>(b2_dynamicBody, sf::Vector2f(1, 1));
        auto pSprite = particle->addComponent<SpriteComponent>();
        pSprite->setTexture(_texture);
        _particles.push_back({particle->getPosition(), _velocity, particle, 0.f, _life});
        //std::cout << "Particle created" << std::endl;

}

void ParticleGenerator::update(double dt) {
    if (!_active) return;
    for (int i = 0; i< _particles.size(); ++i) {
        auto &p = _particles[i];
        p.timer += dt;
        if (p.timer >= p.life) {
            //Delete particle
            p.entity->setForDelete();
            _particles.erase(_particles.begin() + i);
            --i;
            //Check for contact with player
            if (_player != nullptr ) {
                auto playerBounds = _player->get_components<SpriteComponent>().at(0)->getSprite().getGlobalBounds();
                auto particlePos = p.entity->getPosition();
                if (playerBounds.contains(particlePos)) {
                    auto playerPhysics = _player->get_components<PlayerPhysicsComponent>().at(0);
                    playerPhysics->setHealth(playerPhysics->getHealth() - 5);
                }
            }
        }
    }
    if (_rate > 0) {
        _timer += dt;
        if (_timer >= _rate) {
            _timer = 0;
            for (int i = 0; i < _amount; ++i) {
                regenerateParticle();
            }
        }
    }
}


void ParticleGenerator::render() {
    for (auto &p : _particles) {
        p.entity->render();
    }
}

void ParticleGenerator::setTexture(std::shared_ptr<sf::Texture> texture) {
    _texture = std::move(texture);
    for (auto &p : _particles) {
        p.entity->GetCompatibleComponent<SpriteComponent>().at(0)->setTexture(_texture);
    }
}

void ParticleGenerator::setAmount(int amount) {
    _amount = amount;
}

void ParticleGenerator::setLife(float life) {
    _life = life;
}

void ParticleGenerator::setSpeed(float speed) {
    _speed = speed;
}

void ParticleGenerator::setSpread(float spread) {
    _spread = spread;
}

void ParticleGenerator::setPosition(sf::Vector2f position) {
    _position = position;
}

void ParticleGenerator::setVelocity(sf::Vector2f velocity) {
    _velocity = velocity;
}

void ParticleGenerator::activate(bool active) {
    _active = active;
}

// Getters (same as before)

sf::Sprite ParticleGenerator::getParticleSprite() {
    return _particles[0].entity->GetCompatibleComponent<SpriteComponent>().at(0)->getSprite();
}

int ParticleGenerator::getAmount() {
    return _amount;
}

float ParticleGenerator::getLife() {
    return _life;
}

float ParticleGenerator::getSpeed() {
    return _speed;
}

float ParticleGenerator::getSpread() {
    return _spread;
}

sf::Vector2f ParticleGenerator::getPosition() {
    return _position;
}

sf::Vector2f ParticleGenerator::getVelocity() {
    return _velocity;
}

bool ParticleGenerator::isActive() {
    return _active;
}
