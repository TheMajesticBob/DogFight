#include "input_handler.h"
#include "engine.h"
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/constants.inl"

bool InputHandler::_mouseState[sf::Mouse::ButtonCount];
bool InputHandler::_lastMouseState[sf::Mouse::ButtonCount];
float InputHandler::_mouseWheelState[2];
bool InputHandler::keys[sf::Keyboard::KeyCount];
queue<key_event> InputHandler::unhandled_keys;
FKeyDelegate InputHandler::_keyMap[sf::Keyboard::KeyCount][2];
std::vector<FAxisDelStorage> InputHandler::_axisMap[sf::Keyboard::KeyCount];
double InputHandler::mouse_x, InputHandler::mouse_y, InputHandler::previous_x, InputHandler::previous_y;
float InputHandler::timeSinceMouseMovement = 0.0f;
float InputHandler::DeltaTime = 0.0f;

InputHandler::InputHandler()
{
	GetMousePos(&previous_x, &previous_y);

	for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
	{
		_mouseState[i] = false;
		_lastMouseState[i] = false;
	}
}

InputHandler::~InputHandler()
{
}

void InputHandler::Update(float deltaTime)
{
	DeltaTime = deltaTime;
	timeSinceMouseMovement += deltaTime;

	std::copy(std::begin(_mouseState), std::end(_mouseState), std::begin(_lastMouseState));
	for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
	{
		_mouseState[i] = sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
	}

	while (!unhandled_keys.empty()) 
	{
		// Get keys from the queue
		key_event event = unhandled_keys.front();
		unhandled_keys.pop();

		_keyMap[event.key][event.action].invokeSafe();

		// Set a flag for held buttons to run axis functions on them
		keys[event.key] = event.action == sf::Event::KeyPressed;
	}

	// Handle axis
	for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		// If key is not held skip it
		if (!keys[i])
		{
			continue;
		}

		for (FAxisDelStorage& del : _axisMap[i])
		{
			del.del.invokeSafe(del.value);
		}
	}
}

// Gets mouse delta taking screen aspect into account
void InputHandler::GetRealMouseDelta(double* xdelta, double* ydelta)
{
	static double ratio_width = glm::quarter_pi<float>() / static_cast<float>(Engine::GetWindowSize().x);
	static double ratio_height =
		(glm::quarter_pi<float>() *
		(static_cast<float>(Engine::GetWindowSize().y) / static_cast<float>(Engine::GetWindowSize().x))) /
		static_cast<float>(Engine::GetWindowSize().y);

	double dx, dy;
	InputHandler::GetMouseDelta(&dx, &dy);

	*xdelta = dx * ratio_width;
	*ydelta = dy * ratio_height;
}

// Gets mouse delta
void InputHandler::GetMouseDelta(double * xdelta, double * ydelta)
{
	if (timeSinceMouseMovement < 0.05f)
	{
		*xdelta = mouse_x - previous_x;
		*ydelta = mouse_y - previous_y;
	}
	else {
		*xdelta = 0.0f;
		*ydelta = 0.0f;
	}
}

// Gets mouse position
void InputHandler::GetMousePos(double * xpos, double * ypos)
{
	*xpos = mouse_x;
	*ypos = mouse_y;
}

// Handles keyboard input
void InputHandler::KeyboardHandler(int key, int scancode, int action, int mods)
{
	// Just add the necessary data into a queue
	unhandled_keys.emplace(key, scancode, action, mods, std::chrono::steady_clock::now());
}

// Handles mouse position
void InputHandler::MousePosHandler(double xpos, double ypos)
{
	previous_x = mouse_x;
	previous_y = mouse_y;
	mouse_x = xpos;
	mouse_y = ypos;
	timeSinceMouseMovement = 0.0f;
}

void InputHandler::MouseWheelHandler(int wheel, float delta)
{
	_mouseWheelState[wheel] = delta;
}

void InputHandler::LateUpdate(float deltaTime)
{
	for (int i = 0; i < 2; ++i)
	{
		_mouseWheelState[i] = 0.0f;
	}
}
