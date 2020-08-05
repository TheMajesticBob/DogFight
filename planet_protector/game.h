#pragma once
#include "scenes/scene_editor.h"
#include "scenes/scene_testing.h"
#include "scenes/scene_menuMain.h"
#include "scenes/scene_menuSettings.h"

#define GAME_WIDTH 1280
#define GAME_HEIGHT 768

extern TestingScene testing;
extern MainMenu menu;
extern Settings settings;
extern EditorScene editorScene;

extern std::shared_ptr<class AmbientManager> _ambientManager;

namespace Audio
{
	static float MasterVolume = 1.0f;
	static float EffectsVolume = 1.0f;
	static float MusicVolume = 1.0f;
}