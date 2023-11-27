// Shooting component C++ file
#include "cmp_shooting.h"
#include "../drop_pod_game.h"
#include "cmp_actor_movement.h"
#include "cmp_sprite.h"
#include <SFML/Graphics.hpp>

#include "cmp_monster.h"
#include "system_renderer.h"
#include "engine.h"

using namespace std;
using namespace sf;

// ShootingComponent Variables
unsigned int bulletCount;
vector<Bullet> bullets(256);

float tempfloat = 0.f;
shared_ptr<float> angleshot = make_unique<float>(tempfloat);

// Bullet Variables
bool isVisible = false;
float angle;
Vector2f mousePos;

auto temp2 = Texture::Texture();
auto spriteTexture = make_shared<Texture>(temp2);

ShootingComponent::ShootingComponent(Entity* p) : ActorMovementComponent(p) {
	Bullet::init();
}

void ShootingComponent::update(double dt) {
	Bullet::update(dt);
}

void ShootingComponent::render() {
	Bullet::render();
}

void ShootingComponent::Fire() {
	auto spriteSize = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getLocalBounds();
	Vector2f spriteCenter = Vector2f(spriteSize.width * 0.5, spriteSize.height * 0.5);
	// Firing the bullets
	Bullet::fire(_parent->getPosition());
}

Bullet::Bullet()
{
	_damage = 34;
}

void Bullet::init() {
	spriteTexture = Resources::get<Texture>("bulletGlow.png");

	for (auto& b : bullets) {
		b.setPosition(Vector2f(-100, -100));
		b.setTexture(*spriteTexture);
		b.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(spriteTexture->getSize().x, spriteTexture->getSize().y)));
		b.setOrigin(Vector2f(spriteTexture->getSize().x * 0.5, spriteTexture->getSize().y * 0.5));
		b.setAngle(0.f, b);
		b.mousePos = Vector2f(0, 0);
		b.isVisible = false;
	}
}

void Bullet::setAngle(float a, Bullet& b) {
	b.angle = a;
}

void Bullet::update(const double dt) {
	for (auto& b : bullets) {
		b._update(dt);
	}
}

void Bullet::render() {
	for (auto& b : bullets) {
		if (b.isVisible) {
			Renderer::queue(&b);
		}
	}
}

void Bullet::fire(const Vector2f& pos) {
	RenderWindow& window = Engine::GetWindow();

	auto mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	bulletCount++;
	bulletCount = bulletCount % 256;
	bullets[bulletCount].setPosition(pos);
	bullets[bulletCount].isVisible = true;

	// Sets the angle of the bullet.
	*angleshot = atan2(mousePos.y - bullets[bulletCount].getPosition().y, mousePos.x - bullets[bulletCount].getPosition().x);
	bullets[bulletCount].setAngle(*angleshot, bullets[bulletCount]);
}

void Bullet::_update(const double dt) {
	RenderWindow& window = Engine::GetWindow();
	const View view = window.getView();

	// If bullet is out of bounds. remove/return;
	if (getPosition().x < view.getCenter().x - 100 - view.getSize().x * 0.5 || getPosition().x > view.getCenter().x + 100 + view.getSize().x * 0.5
		|| getPosition().y < view.getCenter().y - 100 - view.getSize().y * 0.5 || getPosition().y > view.getCenter().y + 100 + view.getSize().y * 0.5)
	{
		this->isVisible = false;
		return;
	}
	else {
		this->move(cos(this->angle) * 200.f * dt, 0);
		this->move(0, sin(this->angle) * 200.f * dt);
	}

	auto ecm = planetLevel.getEcm();
	auto enemies = ecm.find("enemy");
	auto boundingBox = getGlobalBounds();

	for (auto enemy : enemies)
	{
		auto sprite = enemy->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
		auto spriteBounds = sprite.getGlobalBounds();
		spriteBounds.top += 40;
		spriteBounds.left += 40;
		spriteBounds.width -= 70;
		spriteBounds.height -= 70;
		if (enemy->isAlive() && spriteBounds.intersects(boundingBox))
		{
			this->isVisible = false;
			setPosition(-100, -100);

			// Hit Sound
			soundHit_buffer = Resources::get<SoundBuffer>("Hit.wav");
			soundHit = make_shared<Sound>(*soundHit_buffer);
			soundHit->setVolume(volume);
			soundHit->play();

			auto currentHealth = enemy->GetCompatibleComponent<MonsterComponent>()[0]->get_health();
			enemy->GetCompatibleComponent<MonsterComponent>()[0]->set_health(currentHealth - _damage);
		}
	}
}