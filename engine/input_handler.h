#pragma once
#include <map>
#include <queue>
#include <chrono>

#include <SFML/Window/Keyboard.hpp>

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

struct FAxisDelStorage
{
	FAxisDelStorage() {}

	FAxisDelStorage(FAxisDelegate delegate, float value = 0.0f)
		: del(delegate), value(value)
	{

	}

	FAxisDelStorage& operator=(const FAxisDelStorage &rhs)
	{
		del = rhs.del;
		value = rhs.value;
		return *this;
	}

	const bool operator == (const FAxisDelStorage& rhs) const
	{
		return (del == rhs.del);
	}

	FAxisDelegate del;
	float value;
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
			_keyMap[key][action] += d.getDelegateInternal();
		}

		static void UnbindKey(int key, int action, FKeyDelegate d)
		{
			_keyMap[key][action] -= d.getDelegateInternal();
		}

		static void BindAxis(int key, float value, FAxisDelegate d)
		{
			_axisMap[key].push_back(FAxisDelStorage(d, value));
		}

		static void UnbindAxis(int key, FAxisDelegate d)
		{
			_axisMap[key].erase(std::remove(_axisMap[key].begin(), _axisMap[key].end(), FAxisDelStorage(d)), _axisMap[key].end());
		}
	
	private:		
		static FKeyDelegate _keyMap[sf::Keyboard::KeyCount][2];
		static std::vector<FAxisDelStorage> _axisMap[sf::Keyboard::KeyCount];

		static double mouse_x, mouse_y, previous_x, previous_y;
		static float timeSinceMouseMovement;

		static bool keys[sf::Keyboard::KeyCount];
		static queue<key_event> unhandled_keys;
};