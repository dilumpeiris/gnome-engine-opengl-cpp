#ifndef GNOME_ENGINE_H
#define GNOME_ENGINE_H

#include <iostream>
#include <memory.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ECS.h"
#include "InputHandler.h"

namespace Gnome {

Manager *manager;

// =====================================================================================================
// GnomeEngine Class
// =====================================================================================================

class GnomeEngine {
  public:
	glm::mat4 view;
	glm::mat4 projection;

  public:
	// Constructor and destructor
	GnomeEngine();
	~GnomeEngine();

	// Delete copy constructor and assignment operator
	GnomeEngine(const GnomeEngine &) = delete;
	GnomeEngine &operator=(const GnomeEngine &) = delete;

	// Core engine methods
	bool Initialize(int width = 800, int height = 600, const std::string &title = "GnomeEngine");
	void Run();
	void Shutdown();
	virtual void Render() {};

	// Getters
	bool IsRunning() const { return m_running; }
	GLFWwindow *GetWindow() const { return m_window; }

	// Utility functions
	unsigned int createShaderProgram();

  private:
	// Private methods
	bool InitializeWindow(int width, int height, const std::string &title);
	bool InitializeOpenGL();
	void Update(float deltaTime);
	void ProcessInput();

	// Static callbacks
	static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
	static void ErrorCallback(int error, const char *description);

	// Member variables
	GLFWwindow *m_window;
	bool m_running;

	int m_windowWidth;
	int m_windowHeight;

	std::string m_windowTitle;

	// Timing
	float m_lastFrameTime;
};

// =================================
// GnomeEngine Implementation
// =================================

inline GnomeEngine::GnomeEngine()
    : m_window(nullptr), m_running(false), m_windowWidth(800), m_windowHeight(600),
      m_windowTitle("GnomeEngine"), m_lastFrameTime(0.0f) {}

inline GnomeEngine::~GnomeEngine() { Shutdown(); }

inline bool GnomeEngine::Initialize(int width, int height, const std::string &title) {

	m_windowWidth = width;
	m_windowHeight = height;
	m_windowTitle = title;

	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);

	// Set error callback
	glfwSetErrorCallback(ErrorCallback);

	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	// Initialize window
	if (!InitializeWindow(width, height, title)) {
		glfwTerminate();
		return false;
	}
	// Intialize GLAD to handle function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Set Input Handler
	InputHandler::get().init(m_window);

	// Set initial viewport size
	// This should be run after initializing GLAD as the function pointers will be
	// null and can cause unpredictable behaviour otherwise
	int framebufferWidth, framebufferHeight;
	glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Initialize OpenGL
	if (!InitializeOpenGL()) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	m_running = true;

	std::cout << "GnomeEngine initialized successfully!" << std::endl;
	manager = new Manager();
	return true;
}

inline void GnomeEngine::Run() {

	if (!m_running) {
		std::cerr << "Engine not initialized!" << std::endl;
		return;
	}

	auto lastTime = std::chrono::high_resolution_clock::now();

	// Main game loop.
	while (!glfwWindowShouldClose(m_window) && m_running) {
		glClear(GL_COLOR_BUFFER_BIT);

		// Calculate delta time
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		// Process input
		ProcessInput();
		glfwPollEvents();

		// Update game logic
		Update(deltaTime);

		Render();

		manager->update();
		manager->draw();

		// Swap buffers and poll events
		glfwSwapBuffers(m_window);
	}
}

inline void GnomeEngine::Shutdown() {

	if (m_window) {
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

	glfwTerminate();

	m_running = false;

	std::cout << "GnomeEngine shut down" << std::endl;
}

inline bool GnomeEngine::InitializeWindow(int width, int height, const std::string &title) {

	// Set OpenGL version (3.3 Core)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create window
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		return false;
	}

	// Make context current
	glfwMakeContextCurrent(m_window);

	// Set callbacks
	glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
	//
	// Enable vsync
	glfwSwapInterval(1);

	return true;
}

inline bool GnomeEngine::InitializeOpenGL() {

	// Enable depth testing
	// glEnable(GL_DEPTH_TEST);

	// Set clear color (dark gray)
	glClearColor(0.5, 0.2f, 0.3f, 1.0f);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

	return true;
}

inline void GnomeEngine::Update(float deltaTime) {
	InputHandler::get().update();

	// Update game logic here
	// For now, just store the delta time
	m_lastFrameTime = deltaTime;
}

inline void GnomeEngine::ProcessInput() {

	// Handle ESC key to close window
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}
}

inline void GnomeEngine::FramebufferSizeCallback(GLFWwindow *window, int width, int height) {

	glViewport(0, 0, width, height);
}

inline void GnomeEngine::ErrorCallback(int error, const char *description) {

	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}
}; // namespace Gnome
#endif // GNOME_ENGINE_H
