#pragma once
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

// -------------------------------------------------------
// 						Declaration
// -------------------------------------------------------
class InputHandler {
  public:
	std::queue<KeyEvent> m_keyEventQueue;

	std::unordered_map<int, KeyState> m_keyStates;

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

  private:
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

// -------------------------------------------------------
// 						Implementation
// -------------------------------------------------------
void InputHandler::init(GLFWwindow *window) { glfwSetKeyCallback(window, keyCallback); }
void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	get().m_keyEventQueue.push({key, scancode, action, mods});
}
void InputHandler::update() {

	// Aging the key states.
	// This is done also to mitigate OS key held delay.
	for (auto &[key, state] : m_keyStates) {
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
}

bool InputHandler::isKeyDown(int key) { return get().m_keyStates[key] == KeyState::Down; }
bool InputHandler::isKeyUp(int key) { return get().m_keyStates[key] == KeyState::Up; }
bool InputHandler::isKeyHeld(int key) { return get().m_keyStates[key] == KeyState::Held; }
