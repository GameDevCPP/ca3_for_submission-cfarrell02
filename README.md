
# Deadly Rain Game for C++

This document outlines the components developed for the 'Deadly Rain' CA.

## Player Physics Component

**PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const Vector2f& size, int health)**

- **Size:** Size of the player.
- **Health:** Starting health of the player.

## Pickup Component

**PickupComponent::PickupComponent(Entity* p, PickupType type, int value, std::shared_ptr<Entity> player)**

- **Type:** Can be either HEALTH or SCORE. If HEALTH, it adds health to the player; with SCORE, it adds score to the player.
- **Value:** The amount to be added to health or score.
- **Player Reference:** Used for detection purposes.

## Hazard Component

**HazardComponent::HazardComponent(Entity* p, int damage, std::shared_ptr<Entity> player, float rate)**

- **Damage:** The amount of damage the hazard inflicts.
- **Player Reference:** Used for detection purposes.
- **Rate:** The rate at which damage should be administered.

## Particle Generator Component

**ParticleGenerator::ParticleGenerator(Entity* p, int amount, float life, float speed, float spread, sf::Vector2f position, std::shared_ptr<sf::Texture> texture, std::shared_ptr<Entity> player, float rate, bool loop)**

- **Amount:** Number of particles to be generated.
- **Life:** Lifespan of particles.
- **Speed:** Speed of particles.
- **Spread:** Spread of particles.
- **Position:** Generator's position.
- **Texture:** Texture of particles.
- **Player Reference:** Used for detection.
- **Rate:** Rate of particle generation.
- **Loop:** Boolean for looping the particles.

## Next Level

**NextLevelComponent::NextLevelComponent(Entity* p, std::shared_ptr<Entity> player, std::shared_ptr<Scene> scene, bool isEndGame)**

- **Player Reference:** Used for detection.
- **Scene Reference:** Reference to the next scene.
- **IsEndGame:** Boolean indicating if the current scene is the last level.

## Floating Platforms

**FloatingPlatformComponent::FloatingPlatformComponent(Entity* p, sf::Vector2f startPos, sf::Vector2f endPos, float speed)**

- **StartPos:** Starting position of the platform where it reverses direction.
- **EndPos:** Ending position of the platform where it reverses direction.
- **Speed:** Speed of the platform.


