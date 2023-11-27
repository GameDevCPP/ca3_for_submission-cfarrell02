#pragma once

#include <SFML/Graphics.hpp>
#include <maths.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

#define ls LevelSystem

class LevelSystem {
public:
	static void loadLevelFile(const std::string&, float tileSize = 100.0f);
	static void unload();
	static void render(sf::RenderWindow& window);

	static void renderFloor(sf::RenderWindow& window);

	typedef unsigned char Tile;

	static sf::Texture floorTexture;
	static sf::IntRect floorTextureRect;

	static sf::Texture wallTexture;
	static sf::IntRect wallTextureRect;

	enum TILES {
		EMPTY = ' ',
		START = 's',
		END = 'e',
		WALL = 'w',
		ENEMY = 'n',
		WAYPOINT = '+',
		FLOOR = 'f'
	};

	static Tile getTile(sf::Vector2ul);

	static Tile getTileAt(sf::Vector2f);

	static bool isOnGrid(sf::Vector2f);

	static size_t getWidth();

	static size_t getHeight();

	static sf::Vector2f getTilePosition(sf::Vector2ul);

	static std::vector<sf::Vector2ul> findTiles(Tile);

	static sf::Color getColor(Tile t);

	static void setColor(Tile t, sf::Color c);

	static void setOffset(const sf::Vector2f& _offset);

	static const sf::Vector2f& getOffset();

	static float getTileSize();

	static void setTextureMap(std::string path);

protected:
	static std::unique_ptr<Tile[]> _tiles;
	static size_t _width;
	static size_t _height;
	static sf::Vector2f _offset;

	static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;

	static void buildSprites(bool optimise = true);

	static float _tileSize; // for rendering
	static std::map<Tile, sf::Color> _colours;

private:
	LevelSystem() = delete;

	~LevelSystem() = delete;
};
