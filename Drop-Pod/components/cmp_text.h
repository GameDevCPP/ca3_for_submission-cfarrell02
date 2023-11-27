#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class TextComponent : public Component {
public:
	int sizeOfText;
	TextComponent() = delete;
	explicit TextComponent(Entity* p, float x, float y, const std::string& str = "");
	void update(double dt) override;

	void render() override;

	~TextComponent() override = default;

	void SetText(const std::string& str);
	void setTextSize(int sizeOfText);

protected:
	std::shared_ptr<sf::Font> _font;
	std::string _string;
	sf::Text _text;
};
