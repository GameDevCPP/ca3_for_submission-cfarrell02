
#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_death.h"
#include "scenes/scene_level3.h"

extern MenuScene menu;
extern DeathScene death;

extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;

extern int playerHealth;
extern int playerScore;