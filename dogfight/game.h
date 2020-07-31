#pragma once
#include "scenes/scene_testing.h"
#include "scenes/scene_menuMain.h"
#include "scenes/scene_menuSettings.h"

extern TestingScene testing;
extern MainMenu menu;
extern Settings settings;

extern std::shared_ptr<class AmbientManager> _ambientManager;

static float MasterVolume = 100.0f;
static float SoundVolume = 100.0f;