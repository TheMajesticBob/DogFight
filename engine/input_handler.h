#pragma once
#include <map>
#include <queue>
#include <chrono>
#include "delegates.h"

using namespace std;

DEFINE_DELEGATE(FKeyDelegate);
DEFINE_DELEGATE(FAxisDelegate, float);

struct key_event {
	int key, code, action, modifiers;
	std::chrono::steady_clock::time_point time_of_event;

	key_event(int k, int c, int a, int m, std::chrono::steady_clock::time_point t)
	{
		key = k;
		code = c;
		action = a;
		modifiers = m;
		time_of_event = t;
	}
};

class InputHandler
{
	public:
		InputHandler();
		~InputHandler();

		static void GetRealMouseDelta(double* xdelta, double* ydelta);
		static void GetMouseDelta(double* xdelta, double* ydelta);
		static void GetMousePos(double* xpos, double* ypos);

		// Input callbacks
		static void KeyboardHandler(int key, int scancode, int action, int mods);
		static void MousePosHandler(double xpos, double ypos);
		static void MouseButtonHandler(int button, int action, int mods);

		static void Update(float deltaTime);
		static float DeltaTime;

		// Method binding a key action to a delegate
		static void BindKey(int key, int action, FKeyDelegate d)
		{
			keyMap.emplace(key, pair<int, FKeyDelegate>(action, d));
		}

		static void BindAxis(int key, float value, FAxisDelegate d)
		{
			axisMap.emplace(key, pair<float, FAxisDelegate>(value, d));
		}
	
	private:
		// Map holding pairs of key actions and their respective methods called when key state changes
		static multimap<int, pair<int, FKeyDelegate>> keyMap;
		static multimap<int, pair<float, FAxisDelegate>> axisMap;

		static double mouse_x, mouse_y, previous_x, previous_y;
		static float timeSinceMouseMovement;

		static map<int, bool> keys;
		static queue<key_event> unhandled_keys;
};