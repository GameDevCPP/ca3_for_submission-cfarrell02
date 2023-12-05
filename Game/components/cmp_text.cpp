#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {
}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");

  _text.setPosition(_parent->getPosition());
    _text.setFont(*_font);
    std::cout<<_text.getLocalBounds().width<<", "<<_text.getLocalBounds().height<<std::endl;
  _text.setOrigin(_text.getLocalBounds().width / 2.f,
                  _text.getLocalBounds().height / 2.f);
}

void TextComponent::SetText(const std::string& str) {
  _string = str;
  _text.setString(_string);
}

void TextComponent::SetPosition(const sf::Vector2f& pos) {
  _text.setPosition(pos);
}

void TextComponent::SetCharacterSize(const unsigned int& size) {
  _text.setCharacterSize(size);
}

void TextComponent::SetColor(const sf::Color& color) {
  _text.setFillColor(color);
}