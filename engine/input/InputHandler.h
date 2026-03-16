#pragma once
#include <iostream>
#include <queue>
#include <functional>
#include <GLFW/glfw3.h>
#include <unordered_map>

using KeyCallback = std::function<void(int key)>;

// -------------------------------------------------------
// 			Enums
// -------------------------------------------------------

enum class KeyState { Up, Down, Held, Released };

// -------------------------------------------------------
// 			Structs
// -------------------------------------------------------

struct KeyEvent {
	int key;
	int scancode;
	int action;
	int mods;
};

struct MouseEvent {
	int button;
	int action;
	int mods;
};

// -------------------------------------------------------
// 						Declaration
// -------------------------------------------------------
class InputHandler {
  public:
	std::queue<KeyEvent> m_keyEventQueue;
	std::queue<MouseEvent> m_mouseEventQueue;

	std::unordered_map<int, KeyState> m_keyStates;
	std::unordered_map<int, KeyState> m_mouseStates;

	double mouseX = 0, mouseY = 0;
	double lastMouseX = 0, lastMouseY = 0;
	double mouseDeltaX = 0, mouseDeltaY = 0;
	double scrollIndex = 0;

	// Singleton Instance Imported everywhere.
	static InputHandler &get() {
		static InputHandler instance;
		return instance;
	}

	void init(GLFWwindow *window);
	void update();

	InputHandler() {}
	~InputHandler() {}

	bool isKeyDown(int key);
	bool isKeyUp(int key);
	bool isKeyHeld(int key);
	bool isKeyReleased(int key);

	bool isMouseDown(int button);
	bool isMouseUp(int button);
	bool isMouseHeld(int button);
	bool isMouseReleased(int button);

  private:
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	static void cursorCallback(GLFWwindow *window, double xpos, double ypos);
};

// -------------------------------------------------------
// 						Implementation
// -------------------------------------------------------
void InputHandler::init(GLFWwindow *window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
}
void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	get().m_keyEventQueue.push({key, scancode, action, mods});
}
void InputHandler::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
	get().m_mouseStates[button] = action == GLFW_PRESS ? KeyState::Down : KeyState::Released;
}
void InputHandler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	get().scrollIndex += yoffset;
}
void InputHandler::cursorCallback(GLFWwindow *window, double xpos, double ypos) {
	get().mouseX = xpos;
	get().mouseY = ypos;
}

void InputHandler::update() {

	// Calculate mouse deltas for the current frame
	get().mouseDeltaX = get().mouseX - get().lastMouseX;
	get().mouseDeltaY = get().mouseY - get().lastMouseY;
	get().lastMouseX = get().mouseX;
	get().lastMouseY = get().mouseY;

	// Aging the key states.
	// This is done also to mitigate OS key held delay.
	for (auto &[key, state] : m_keyStates) {
		if (state == KeyState::Down) {
			state = KeyState::Held;
		} else if (state == KeyState::Released) {
			state = KeyState::Up;
		}
	}

	// Aging the mouse states.
	for (auto &[button, state] : m_mouseStates) {
		if (state == KeyState::Down) {
			state = KeyState::Held;
		} else if (state == KeyState::Released) {
			state = KeyState::Up;
		}
	}

	// Processing the key events.
	while (!m_keyEventQueue.empty()) {
		KeyEvent event = m_keyEventQueue.front();
		m_keyEventQueue.pop();

		switch (event.action) {
		case GLFW_PRESS:
			m_keyStates[event.key] = KeyState::Down;
			break;
		case GLFW_RELEASE:
			m_keyStates[event.key] = KeyState::Released;
			break;

		// The OS is responsible for sending the repeat event.
		// This is updated after a short delay.
		case GLFW_REPEAT:
			m_keyStates[event.key] = KeyState::Held;
			break;
		}
	}

	// Processing the mouse events.
	while (!m_mouseEventQueue.empty()) {
		MouseEvent event = m_mouseEventQueue.front();
		m_mouseEventQueue.pop();

		switch (event.action) {
		case GLFW_PRESS:
			m_mouseStates[event.button] = KeyState::Down;
			break;
		case GLFW_RELEASE:
			m_mouseStates[event.button] = KeyState::Released;
			break;
		case GLFW_REPEAT:
			m_mouseStates[event.button] = KeyState::Held;
			break;
		}
	}
}

bool InputHandler::isKeyDown(int key) { return get().m_keyStates[key] == KeyState::Down; }
bool InputHandler::isKeyUp(int key) { return get().m_keyStates[key] == KeyState::Up; }
bool InputHandler::isKeyHeld(int key) { return get().m_keyStates[key] == KeyState::Held; }
bool InputHandler::isKeyReleased(int key) { return get().m_keyStates[key] == KeyState::Released; }

bool InputHandler::isMouseDown(int button) { return get().m_mouseStates[button] == KeyState::Down; }
bool InputHandler::isMouseUp(int button) { return get().m_mouseStates[button] == KeyState::Up; }
bool InputHandler::isMouseHeld(int button) { return get().m_mouseStates[button] == KeyState::Held; }
bool InputHandler::isMouseReleased(int button) {
	return get().m_mouseStates[button] == KeyState::Released;
}
