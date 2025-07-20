#ifndef GNOME_ENGINE_H
#define GNOME_ENGINE_H

#include <string.h>
#include <sstream>
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

// TODO: Do you want to seperate and write the code cleanly?
// TODO: Flush out the GRect and create the GTriangle
// TODO: Add GCircle.
// TODO: A geometric shape can have a border and a fill color.
namespace Gnome {

Manager * manager;

void printMat4(const glm::mat4& mat) {
    for (int row = 0; row < 4; ++row) {
        std::cout << "[ ";
        for (int col = 0; col < 4; ++col) {
            std::cout << mat[col][row] << " "; // glm is column-major
        }
        std::cout << "]\n";
    }
}

// =====================================================================================================
// Shader Component
// =====================================================================================================
class Shader: public Component {
public:
	unsigned int ID;
	
	const char * vertexSrc;
	const char * fragmentSrc;

public:
	Shader() {
		// m:shader
		vertexSrc = R"(
			#version 330 core
	
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;
			layout (location = 3) in vec2 aTexCoord;
	
			out vec3 ourColor;
			out vec2 TexCoord;
	
			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
	
			uniform mat4 transform;
	
	
			void main() {
	
			gl_Position = projection * view * model * vec4(aPos, 1.0);
	
			ourColor = aColor;
			TexCoord = aTexCoord;
	
			}
		)";

		fragmentSrc = R"(
			#version 330 core
	
			out vec4 FragColor;
			
			in vec3 ourColor;
			in vec2 TexCoord;
	
			uniform sampler2D texture1;
			uniform sampler2D texture2;
	
			void main() {
	
			vec4 color1 = texture(texture1, TexCoord);
			vec4 color2 = texture(texture2, TexCoord);
			
			 FragColor = mix(color1, color2, 0.5); 
			//  FragColor = vec4(ourColor, 1.0);
			}
		)";

		useShaders(vertexSrc, fragmentSrc);
		
	}
	void useShaders(const char* vertexShaderSrc, const char * fragmentShaderSrc) {

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
		glShaderSource(vertex_shader, 1, &vertexSrc, NULL);
		glShaderSource(fragment_shader, 1, &fragmentSrc, NULL);
	
		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);
	
		ID = glCreateProgram();
	
		glAttachShader(ID, vertex_shader);
		glAttachShader(ID, fragment_shader);
	
		glLinkProgram(ID);
	
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	void use() {
		glUseProgram(ID);
	}
	void update()override {}

};

// =====================================================================================================
// Transform Component
// =====================================================================================================

class Transform: public Component {
public:
	unsigned int shader;
	
	glm::mat4 position;
	glm::mat4 rotation;
	// glm::mat4 scale;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	unsigned int shaderID;

	unsigned int shader_model;
	unsigned int shader_view;
	unsigned int shader_projection;

public:
	Transform (unsigned int shaderID) {

		this->shaderID = shaderID;

		shader_model = glGetUniformLocation(shaderID, "model");
		shader_view = glGetUniformLocation(shaderID, "view");
		shader_projection = glGetUniformLocation(shaderID, "projection");

		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1024.0f/768.0f, 0.1f, 100.0f);
		
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	}

	void update() override {
		glUniformMatrix4fv(shader_model, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shader_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shader_projection, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void translate(float x, float y, float z){
		view = glm::translate(view, glm::vec3(x, y, z));
	}

	void rotate(float angle, float x, float y, float z) {
		
	}

	void scale(float x, float y, float z) {}
};

struct Texture {
	unsigned int ID;
	const char * name;
	float transparency;
};

// =====================================================================================================
// Material Component
// =====================================================================================================
class Material: public Component {

public:
	unsigned int ID;

	std::vector<Texture> texture_items;	
	int currentTexLocation = 0;

public:
	Material() {
	
	}

	void addTexture(const char* filePath,  bool repeat_x = false, bool repeat_y = false, bool flipped = false) {
		Texture text;
		text.name = "texture00";

		int width, height, nrChannels;
		GLenum format;
		unsigned char* data = loadImage(filePath, &width, &height, &nrChannels, &format, flipped);

		glGenTextures(1, &text.ID);
		glBindTexture(GL_TEXTURE_2D, text.ID);

		if (repeat_x) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		}

		if (repeat_y) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		texture_items.emplace_back(text);

		currentTexLocation++;
		stbi_image_free(data);

	}
unsigned char* loadImage(const char *fileName, int *width, int *height, int *nrChannels, GLenum *format, bool flipped= false) {

		std::stringstream ss;
		ss << "../assets/" << fileName;
		std::string temp = ss.str();
		const char *filePath = temp.c_str();
	
		stbi_set_flip_vertically_on_load(!flipped);
	
		unsigned char *data = stbi_load(filePath, width, height, nrChannels, 0);
	
		if (data) {
			*format = (*nrChannels == 4) ? GL_RGBA : GL_RGB;
		} else {
			std::cout << "Error: Failed to load to texture:" << filePath << std::endl;
			std::cout << "Reason: " << stbi_failure_reason() << std::endl;
		}
	return data;
	}
	
	void setShaderTextures(unsigned int shaderID){
		glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
		glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
	}

	void bind() {
		for(int i = 0; i < currentTexLocation; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture_items[i].ID);
		}
	}

//	void update()override {std::cout << "this is texture" << std::endl;}
};

// =====================================================================================================
// GRectangle
// =====================================================================================================
class GRect: public Entity {
public:
	unsigned int VAO;

	Transform * transform;
	Shader * shader;
	Material * texture;


public:
	GRect(): GRect(0, 0, 0, 0) {}
	GRect(int x, int y, int width, int height): Entity(0) , transform(0){
		int screen_width = 1024;
		int screen_height = 768;

		// glfwGetFramebufferSize(window, &width, &height);
		float pos_x, pos_y;
		float size_x, size_y;
		
		pos_x = x / (screen_width / 2.0f);
		pos_y = y / (screen_height / 2.0f);
		size_x = width / (screen_width / 2.0f);
		size_y = height / (screen_height / 2.0f);

		float vertices[] = {
		    // positions // colors // texture coords
		    pos_x + size_x / 2.0f,
		    pos_y + size_y / 2.0f,
		    0.0f,
		    1.0f,
		    0.0f,
		    0.0f,
		    1.0f,
		    1.0f, // top right

		    pos_x + size_x / 2.0f,
		    pos_y - size_y / 2.0f,
		    0.0f,
		    0.0f,
		    1.0f,
		    0.0f,
		    1.0f,
		    0.0f, // bottom right

		    pos_x - size_x / 2.0f,
		    pos_y - size_y / 2.0f,
		    0.0f,
		    0.0f,
		    0.0f,
		    1.0f,
		    0.0f,
		    0.0f, // bottom left

		    pos_x - size_x / 2.0f,
		    pos_y + size_y / 2.0f,
		    0.0f,
		    1.0f,
		    1.0f,
		    0.0f,
		    0.0f,
		    1.0f // top left
		};

		int indices[] = {0, 1, 3, 1, 2, 3};

		unsigned int VBO;
		unsigned int EBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		                      (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Material coordinates
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		                      (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		this->addComponent<Shader>();
		shader = this->getComponent<Shader>();

		this->addComponent<Transform>(shader->ID);
	 	this->addComponent<Material>();

		transform = this->getComponent<Transform>();
		texture = this->getComponent<Material>();

		texture->addTexture("crate.jpg");
		texture->addTexture("tnt.jpg");
}

void draw() override {
	shader->use();

	texture->setShaderTextures(shader->ID);
	texture->bind();
	
	transform->update();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
};

// =====================================================================================================
// GnomeEngine Class
// =====================================================================================================
class GnomeEngine {
  public:
	glm::mat4 view;
	glm::mat4  projection;

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
	virtual void Render() {
	};

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
      m_windowTitle("GnomeEngine"), m_lastFrameTime(0.0f) {
}

inline GnomeEngine::~GnomeEngine() { Shutdown(); }

inline bool GnomeEngine::Initialize(int width, int height, const std::string &title) {
	m_windowWidth = width;
	m_windowHeight = height;
	m_windowTitle = title;

	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1024.0f/768.0f, 0.1f, 100.0f);

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

		// Update game logic
		Update(deltaTime);

		Render();

		manager->update();	
		manager->draw();

		// Swap buffers and poll events
		glfwSwapBuffers(m_window);
		glfwPollEvents();
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
