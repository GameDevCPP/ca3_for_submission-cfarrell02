#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include <JsonReader.h>
#include <SFML/Graphics.hpp>



using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
DeathScene death;

int playerHealth = 100;
int playerScore = 0;


int main() {
    Engine::Start(1920 , 1080, "Deadly Rain", &menu);
    //JsonReader::loadLevel("res/level1.json");
}