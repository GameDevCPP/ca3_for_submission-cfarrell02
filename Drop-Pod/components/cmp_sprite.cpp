// Sprite, Shape and Animation components C++ file
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;
using namespace sf;

// Sprite ---------------------------------------------------------------------------------------------------------------
void SpriteComponent::setTexture(shared_ptr<Texture> tex)
{
	_texture = tex;
	_sprite->setTexture(*_texture);
}

shared_ptr<Texture> SpriteComponent::getTexture()
{
	return _texture;
}

void SpriteComponent::setTextureRect(sf::IntRect rect)
{
	_sprite->setTextureRect(rect);
}

SpriteComponent::SpriteComponent(Entity* p)
	: Component(p), _sprite(make_shared<Sprite>()) {}

void SpriteComponent::update(double dt) {
	_sprite->setPosition(_parent->getPosition());
	_sprite->setRotation(deg2rad(_parent->getRotation()));
}

// Shape ---------------------------------------------------------------------------------------------------------------
void SpriteComponent::render() { Renderer::queue(_sprite.get()); }

void ShapeComponent::update(double dt) {
	_shape->setPosition(_parent->getPosition());
	_shape->setRotation(deg2rad(_parent->getRotation()));
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
	: Component(p), _shape(make_shared<CircleShape>()) {}

Sprite& SpriteComponent::getSprite() const { return *_sprite; }

// Animation ----------------------------------------------------------------------------------------------------------
AnimationComponent::AnimationComponent(Entity* p) : Component(p), frameCount(0), _row(0)
{
	totalProgress = 0.f;
	totalLength = 0.f;
}

void AnimationComponent::addFrame(Frame& frame) {
	frames.push_back(move(frame));
	totalLength += frame.duration;
}

void AnimationComponent::setAnimation(int size, float duration, shared_ptr<Texture> texture, IntRect rect)
{
	_parent->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(texture);

	for (auto i = 0; i < size; i++)
	{
		Frame tempFrame;
		auto width = rect.width;
		tempFrame.rect = rect;
		tempFrame.rect.left = width * i;
		tempFrame.duration = duration;
		addFrame(tempFrame);
	}
}

void AnimationComponent::setRow(int row) {
	frameCount = 0;
	_row = row;

	for (Frame& frame : frames)
	{
		auto top = frame.rect.height * row;
		frame.rect.top = top;
	}
}

int AnimationComponent::getRow() const
{
	return _row;
}

void AnimationComponent::setDuration(float duration)
{
	totalLength = 0.f;
	for (Frame& frame : frames)
	{
		frame.duration = duration;
		totalLength += duration;
	}
}

void AnimationComponent::update(double dt) {
	totalProgress += dt;
	if (totalProgress >= totalLength / frames.size())
	{
		frameCount++;

		if (frameCount >= frames.size()) {
			frameCount = 0;
		}

		_parent->GetCompatibleComponent<SpriteComponent>()[0]->setTextureRect(frames[frameCount].rect);
		totalProgress = 0.f;
	}
}

void AnimationComponent::render() {}