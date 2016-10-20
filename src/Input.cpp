#include "Input.h"
#include "Window\WindowManager.h"

Input* Input::_instance = nullptr;
Window* Input::_window = nullptr;
std::map<int, bool> Input::_currentKeys;
std::map<int, bool> Input::_previousKeys;
bool Input::_currentMouse[3];
bool Input::_previousMouse[3];

void Input::Init()
{
	_window = WindowManager::GetInstance()->GetMainWindow();
}

void Input::Update()
{
	for (int i = 0; i < NUM_KEYS; i++)
		_previousKeys[_keys[i]] = _currentKeys[_keys[i]];

	for (int i = 0; i < NUM_KEYS; i++)
		_currentKeys[_keys[i]] = _window->GetKey((KeyCode)_keys[i]) == GLFW_PRESS;

	for (int i = 0; i < NUM_BUTTONS; i++)
		_previousMouse[i] = _currentMouse[i];

	for (int i = 0; i < NUM_BUTTONS; i++)
		_currentMouse[i] = _window->GetMouse((ButtonCode)i) == GLFW_PRESS;
}

void Input::Shutdown()
{

}

bool Input::GetKey(KeyCode key)
{
	return _currentKeys[key];
}

bool Input::GetKeyDown(KeyCode key)
{
	return _currentKeys[key] && !_previousKeys[key];
}

bool Input::GetKeyUp(KeyCode key)
{
	return !_currentKeys[key] && _previousKeys[key];
}

bool Input::GetMouse(ButtonCode button)
{
	return _currentMouse[button];
}

bool Input::GetMouseDown(ButtonCode button)
{
	return _currentMouse[button] && !_previousMouse[button];
}

bool Input::GetMouseUp(ButtonCode button)
{
	return !_currentMouse[button] && _previousMouse[button];
}

int Input::_keys[] =
{
	GLFW_KEY_ESCAPE,
	GLFW_KEY_F1,
	GLFW_KEY_F2,
	GLFW_KEY_F3,
	GLFW_KEY_F4,
	GLFW_KEY_F5,
	GLFW_KEY_F6,
	GLFW_KEY_F7,
	GLFW_KEY_F8,
	GLFW_KEY_F9,
	GLFW_KEY_F10,
	GLFW_KEY_F11,
	GLFW_KEY_F12,
	GLFW_KEY_PRINT_SCREEN,
	GLFW_KEY_SCROLL_LOCK,
	GLFW_KEY_PAUSE,

	GLFW_KEY_GRAVE_ACCENT,
	GLFW_KEY_1,
	GLFW_KEY_2,
	GLFW_KEY_3,
	GLFW_KEY_4,
	GLFW_KEY_5,
	GLFW_KEY_6,
	GLFW_KEY_7,
	GLFW_KEY_8,
	GLFW_KEY_9,
	GLFW_KEY_0,
	GLFW_KEY_MINUS,
	GLFW_KEY_EQUAL,
	GLFW_KEY_BACKSPACE,
	GLFW_KEY_INSERT,
	GLFW_KEY_HOME,
	GLFW_KEY_PAGE_UP,

	GLFW_KEY_TAB,
	GLFW_KEY_Q,
	GLFW_KEY_W,
	GLFW_KEY_E,
	GLFW_KEY_R,
	GLFW_KEY_T,
	GLFW_KEY_Y,
	GLFW_KEY_U,
	GLFW_KEY_I,
	GLFW_KEY_O,
	GLFW_KEY_P,
	GLFW_KEY_LEFT_BRACKET,
	GLFW_KEY_RIGHT_BRACKET,
	GLFW_KEY_BACKSLASH,
	GLFW_KEY_DELETE,
	GLFW_KEY_END,
	GLFW_KEY_PAGE_DOWN,

	GLFW_KEY_CAPS_LOCK,
	GLFW_KEY_A,
	GLFW_KEY_S,
	GLFW_KEY_D,
	GLFW_KEY_F,
	GLFW_KEY_G,
	GLFW_KEY_H,
	GLFW_KEY_J,
	GLFW_KEY_K,
	GLFW_KEY_L,
	GLFW_KEY_SEMICOLON,
	GLFW_KEY_APOSTROPHE,
	GLFW_KEY_ENTER,

	GLFW_KEY_LEFT_SHIFT,
	GLFW_KEY_Z,
	GLFW_KEY_X,
	GLFW_KEY_C,
	GLFW_KEY_V,
	GLFW_KEY_B,
	GLFW_KEY_N,
	GLFW_KEY_M,
	GLFW_KEY_COMMA,
	GLFW_KEY_PERIOD,
	GLFW_KEY_SLASH,
	GLFW_KEY_RIGHT_SHIFT,

	GLFW_KEY_LEFT_CONTROL,
	GLFW_KEY_LEFT_ALT,
	GLFW_KEY_SPACE,
	GLFW_KEY_RIGHT_ALT,
	GLFW_KEY_RIGHT_CONTROL,

	GLFW_KEY_UP,
	GLFW_KEY_LEFT,
	GLFW_KEY_DOWN,
	GLFW_KEY_RIGHT,

	GLFW_KEY_NUM_LOCK,
	GLFW_KEY_KP_DIVIDE,
	GLFW_KEY_KP_MULTIPLY,
	GLFW_KEY_KP_SUBTRACT,
	GLFW_KEY_KP_7,
	GLFW_KEY_KP_8,
	GLFW_KEY_KP_9,
	GLFW_KEY_KP_4,
	GLFW_KEY_KP_5,
	GLFW_KEY_KP_6,
	GLFW_KEY_KP_ADD,
	GLFW_KEY_KP_1,
	GLFW_KEY_KP_2,
	GLFW_KEY_KP_3,
	GLFW_KEY_KP_0,
	GLFW_KEY_KP_DECIMAL,
	GLFW_KEY_KP_ENTER
};

int Input::_buttons[] =
{
	ButtonCode::Left,
	ButtonCode::Right,
	ButtonCode::Middle,
};