#pragma once
#include <glfw\glfw3.h>
#include <map>

enum KeyCode
{
	Escape = GLFW_KEY_ESCAPE,
	F1 = GLFW_KEY_F1,
	F2 = GLFW_KEY_F2,
	F3 = GLFW_KEY_F3,
	F4 = GLFW_KEY_F4,
	F5 = GLFW_KEY_F5,
	F6 = GLFW_KEY_F6,
	F7 = GLFW_KEY_F7,
	F8 = GLFW_KEY_F8,
	F9 = GLFW_KEY_F9,
	F10 = GLFW_KEY_F10,
	F11 = GLFW_KEY_F11,
	F12 = GLFW_KEY_F12,
	PrintScreen = GLFW_KEY_PRINT_SCREEN,
	ScrollLock = GLFW_KEY_SCROLL_LOCK,
	Pause = GLFW_KEY_PAUSE,

	Grave = GLFW_KEY_GRAVE_ACCENT,
	Number1 = GLFW_KEY_1,
	Number2 = GLFW_KEY_2,
	Number3 = GLFW_KEY_3,
	Number4 = GLFW_KEY_4,
	Number5 = GLFW_KEY_5,
	Number6 = GLFW_KEY_6,
	Number7 = GLFW_KEY_7,
	Number8 = GLFW_KEY_8,
	Number9 = GLFW_KEY_9,
	Number0 = GLFW_KEY_0,
	Dash = GLFW_KEY_MINUS,
	Equals = GLFW_KEY_EQUAL,
	Backspace = GLFW_KEY_BACKSPACE,
	Insert = GLFW_KEY_INSERT,
	Home = GLFW_KEY_HOME,
	PageUp = GLFW_KEY_PAGE_UP,

	Tab = GLFW_KEY_TAB,
	Q = GLFW_KEY_Q,
	W = GLFW_KEY_W,
	E = GLFW_KEY_E,
	R = GLFW_KEY_R,
	T = GLFW_KEY_T,
	Y = GLFW_KEY_Y,
	U = GLFW_KEY_U,
	I = GLFW_KEY_I,
	O = GLFW_KEY_O,
	P = GLFW_KEY_P,
	LeftBracket = GLFW_KEY_LEFT_BRACKET,
	RightBracket = GLFW_KEY_RIGHT_BRACKET,
	Backslash = GLFW_KEY_BACKSLASH,
	Delete = GLFW_KEY_DELETE,
	End = GLFW_KEY_END,
	PageDown = GLFW_KEY_PAGE_DOWN,

	CapsLock = GLFW_KEY_CAPS_LOCK,
	A = GLFW_KEY_A,
	S = GLFW_KEY_S,
	D = GLFW_KEY_D,
	F = GLFW_KEY_F,
	G = GLFW_KEY_G,
	H = GLFW_KEY_H,
	J = GLFW_KEY_J,
	K = GLFW_KEY_K,
	L = GLFW_KEY_L,
	Semicolon = GLFW_KEY_SEMICOLON,
	Apostrophe = GLFW_KEY_APOSTROPHE,
	Enter = GLFW_KEY_ENTER,

	LeftShift = GLFW_KEY_LEFT_SHIFT,
	Z = GLFW_KEY_Z,
	X = GLFW_KEY_X,
	C = GLFW_KEY_C,
	V = GLFW_KEY_V,
	B = GLFW_KEY_B,
	N = GLFW_KEY_N,
	M = GLFW_KEY_M,
	Comma = GLFW_KEY_COMMA,
	Period = GLFW_KEY_PERIOD,
	Slash = GLFW_KEY_SLASH,
	RightShift = GLFW_KEY_RIGHT_SHIFT,

	LeftControl = GLFW_KEY_LEFT_CONTROL,
	LeftAlt = GLFW_KEY_LEFT_ALT,
	Space = GLFW_KEY_SPACE,
	RightAlt = GLFW_KEY_RIGHT_ALT,
	RightControl = GLFW_KEY_RIGHT_CONTROL,

	UpArrow = GLFW_KEY_UP,
	LeftArrow = GLFW_KEY_LEFT,
	DownArrow = GLFW_KEY_DOWN,
	RightArrow = GLFW_KEY_RIGHT,

	NumLock = GLFW_KEY_NUM_LOCK,
	KeypadDivide = GLFW_KEY_KP_DIVIDE,
	KeypadMultiply = GLFW_KEY_KP_MULTIPLY,
	KeypadMinus = GLFW_KEY_KP_SUBTRACT,
	Keypad7 = GLFW_KEY_KP_7,
	Keypad8 = GLFW_KEY_KP_8,
	Keypad9 = GLFW_KEY_KP_9,
	Keypad4 = GLFW_KEY_KP_4,
	Keypad5 = GLFW_KEY_KP_5,
	Keypad6 = GLFW_KEY_KP_6,
	KeypadPlus = GLFW_KEY_KP_ADD,
	Keypad1 = GLFW_KEY_KP_1,
	Keypad2 = GLFW_KEY_KP_2,
	Keypad3 = GLFW_KEY_KP_3,
	Keypad0 = GLFW_KEY_KP_0,
	KeypadDelete = GLFW_KEY_KP_DECIMAL,
	KeypadEnter = GLFW_KEY_KP_ENTER,
};

enum ButtonCode
{
	Left,
	Right,
	Middle
};

class Window;
class Input
{
public:
	static void Init();
	static void Update();
	static void Shutdown();

	static bool GetKey(KeyCode key);
	static bool GetKeyDown(KeyCode key);
	static bool GetKeyUp(KeyCode key);

	static bool GetMouse(ButtonCode button);
	static bool GetMouseDown(ButtonCode button);
	static bool GetMouseUp(ButtonCode button);
private:
	static Input* _instance;
	static Window* _window;

	static const int NUM_KEYS = 101;
	static std::map<int, bool> _currentKeys;
	static std::map<int, bool> _previousKeys;
	static int _keys[101];

	static const int NUM_BUTTONS = 3;
	static bool _currentMouse[3];
	static bool _previousMouse[3];
	static int _buttons[3];
};

