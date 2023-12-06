#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include <JsonReader.h>
#include <SFML/Graphics.hpp>



using namespace std;

MenuScene menu;
Level1Scene level1;
DeathScene death;


int main() {
    Engine::Start(1920 , 1080, "Platformer", &menu);
    //JsonReader::loadLevel("res/level1.json");
}