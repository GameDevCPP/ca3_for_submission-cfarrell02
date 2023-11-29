#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include <JsonReader.h>



using namespace std;

MenuScene menu;
Level1Scene level1;


int main() {
  Engine::Start(1280, 720, "Platformer", &level1);
  //JsonReader::loadLevel("res/level1.json");
}