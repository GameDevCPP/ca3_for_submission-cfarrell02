#include "LevelSystem.h"
#include <fstream>
#include "JsonReader.h"

using namespace std;
using namespace sf;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colours{
	{WALL, Color::White}, {END, Color::Red}, {FLOOR, Color::Magenta}, {START, Color::Green } };

sf::Color LevelSystem::getColor(LevelSystem::Tile t) {
	auto it = _colours.find(t);
	if (it == _colours.end()) {
		_colours[t] = Color::Transparent;
	}
	return _colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c) {
	_colours[t] = c;
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
// Initialize the static member _texture
sf::Texture LevelSystem::_texture;
std::vector<sf::IntRect> LevelSystem::textures;
LevelSystem::Level LevelSystem::_level;
std::map<string, Texture> LevelSystem::_hazardTextures;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(0.0f, 0.0f);
// Vector2f LevelSystem::_offset(0,0);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

void LevelSystem::loadTextureFile(const std::string &, float tileSize) {
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    // Open tilemap image
    if (!_texture.loadFromFile("res/img/Free/Terrain/Terrain (16x16).png")) {
        throw string("Couldn't load tilemap");
    }
    Texture hazardTexture;
    if(!hazardTexture.loadFromFile("res/img/Free/Traps/Spikes/Idle.png"))
    {
        throw string("Couldn't load spike texture");
    }
    _hazardTextures.insert(std::pair<string, Texture>("spikes", hazardTexture));
    // Get tilemap size
    w = _texture.getSize().x / tileSize;
    h = _texture.getSize().y / tileSize;
    // Create tiles
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            IntRect tileBounds = IntRect(j * tileSize, i * tileSize, tileSize, tileSize);
            cout<<"Adding tile: "<<tileBounds.left<<", "<<tileBounds.top<<endl;
            textures.push_back(tileBounds);
        }
    }
}

void LevelSystem::setTextureMap(string path) {
	//floorTexture.loadFromFile(path);
}

void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
	_tileSize = tileSize;
	size_t w = 0, h = 0;
    _level =  JsonReader::loadLevel(path);
	string buffer = _level.rawMap;
	std::vector<Tile> temp_tiles;
	int widthCheck = 0;
	for (int i = 0; i < buffer.size(); ++i) {
		const char c = buffer[i];
		if (c == '\0') { break; }
		if (c == '\n') { // newline
			if (w == 0) {  // if we haven't written width yet
				w = i;       // set width
			}
			else if (w != (widthCheck - 1)) {
				throw string("non uniform width:" + to_string(h) + " ") + path;
			}
			widthCheck = 0;
			h++; // increment height
		}
		else {
			temp_tiles.push_back((Tile)c);
		}
		++widthCheck;
	}

	if (temp_tiles.size() != (w * h)) {
		throw string("Can't parse level file") + path;
	}
	_tiles = std::make_unique<Tile[]>(w * h);
	_width = w; // set static class vars
	_height = h;
	std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
	cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
	buildSprites();
}

void LevelSystem::buildSprites(bool optimise) {
	_sprites.clear();

	struct tp {
		sf::Vector2f p;
		sf::Vector2f s;
		sf::Color c;
	};
	vector<tp> tps;
	const auto tls = Vector2f(_tileSize, _tileSize);
	for (size_t y = 0; y < _height; ++y) {
		for (size_t x = 0; x < _width; ++x) {
			Tile t = getTile({ x, y });
			if (t == EMPTY) {
				continue;
			}
			tps.push_back({ getTilePosition({x, y}), tls, getColor(t) });
		}
	}

	const auto nonempty = tps.size();
    for (auto& t : tps) {
        auto s = make_unique<sf::RectangleShape>();
        s->setPosition(t.p);
        s->setSize(t.s);

        // Assuming "_texture" is a member variable of type sf::Texture
        s->setTexture(&_texture); // Assign the loaded texture to the sprite

        int xVal = (int) t.p.x/t.s.x;
        int yVal = (int) t.p.y/t.s.y;
        auto charVal = _level.map[yVal][xVal];
        int index = _level.tileMap[charVal];
        if (index < 0){
            // Switch statement for hazards
            switch (index) {
                case -1:
                    continue;
                case -2:
                    s->setTexture(&_hazardTextures["spikes"]);
                    s->setTextureRect({0,0,16,16});
                    break;
                default:
                    break;
            }
            _sprites.push_back(std::move(s));
            continue;
        }

        s->setTextureRect(textures[index]);

        _sprites.push_back(std::move(s));
    }
	cout << "Level with " << (_width * _height) << " Tiles, With " << nonempty
		<< " Not Empty, using: " << _sprites.size() << " Sprites\n";
}

void LevelSystem::render(RenderWindow& window) {
	for (auto& t : _sprites) {
		window.draw(*t);
	}
}

void LevelSystem::renderFloor(RenderWindow& window) {
	// Renders tiles from the level file only if they are on the screen.
	View view = window.getView();
	auto res = view.getSize();
	auto center = view.getCenter();

	for (auto& t : _sprites) {
		if (t->getPosition().x < center.x - (res.x * 0.6) || t->getPosition().x >= center.x + (res.x * 0.6)) {
			continue;
		}
		if (t->getPosition().y < center.y - (res.y * 0.65) || t->getPosition().y >= center.y + (res.y * 0.5)) {
			continue;
		}
		else {
			window.draw(*t);
		}
	}
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
	if (p.x > _width || p.y > _height) {
		throw string("Tile out of range: ") + to_string(p.x) + "," +
			to_string(p.y) + ")";
	}
	return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
	return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) {
	auto v = vector<sf::Vector2ul>();
	for (size_t i = 0; i < _width * _height; ++i) {
		if (_tiles[i] == type) {
			v.push_back({ i % _width, i / _width });
		}
	}

	return v;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) {
	auto a = v - _offset;
	if (a.x < 0 || a.y < 0) {
		throw string("Tile out of range ");
	}
	return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
	auto a = v - _offset;
	if (a.x < 0 || a.y < 0) {
		return false;
	}
	auto p = Vector2ul((v - _offset) / (_tileSize));
	if (p.x > _width || p.y > _height) {
		return false;
	}
	return true;
}

void LevelSystem::setOffset(const Vector2f& _offset) {
	LevelSystem::_offset = _offset;
	buildSprites();
}

void LevelSystem::unload() {
	cout << "LevelSystem unloading\n";
	_sprites.clear();
	_tiles.reset();
	_width = 0;
	_height = 0;
	_offset = { 0, 0 };
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }