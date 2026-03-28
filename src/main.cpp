#include <iostream>
#include <unistd.h>

// Not using this order give a redefinition error.
#include "core/GnomeEngine.h"
#include "input/InputHandler.h"
#include "entities/GRectangle.h"
#include "entities/GLine.h"
#include <string>

class Game : public Gnome::GnomeEngine {
  public:
	// Create a Rectangle object using the prebuilt GRect Entity class.
	GRect *my_rect;
	GLine *line;

	int currentAnimationFrame = 0;
	int maxAnimationFrames = 8;

	float animationTimer = 0.0f;
	float animationSpeedFPS = 20.0f; // Default animation speed

  public:
	void setAnimationSpeed(float fps) { animationSpeedFPS = fps; }

	void setup() {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		std::cout << "Working directory: " << cwd << std::endl;

		// Red rectangle
		my_rect = new GRect(0, 0, 100, 100);
		this->addEntity(my_rect);
		my_rect->material->materials.push_back(MaterialAsset{});
		my_rect->material->materials[0].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		my_rect->material->materials[0].addShaderPass("default", GRectShader::vertexSrc,
		                                              GRectShader::fragmentSrc);
		const char *frag = R"(
					// checker.frag
					#version 330 core

					in vec2 TexCoord;

					// uniform float u_scale;  // controls checker size, try 10.0
					out vec4 fragColor;

					void main() {
						vec2  grid  = floor(TexCoord * 10.0);
						float check = mod(grid.x + grid.y, 2.0);
						fragColor   = vec4(vec3(check), 0.4);
					}
		)";

		my_rect->material->materials[0].addShaderPass("checker", GRectShader::vertexSrc, frag);

		// Green line
		line = new GLine();
		this->addEntity(line);
		line->material->materials.push_back(MaterialAsset{});
		line->material->materials[0].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	void Render() override {
		if (InputHandler::get().isKeyHeld(GLFW_KEY_A))
			line->transform->translateAbsolute(-0.05f, 0.0f, 0.0f);
		if (InputHandler::get().isKeyHeld(GLFW_KEY_W))
			line->transform->translateAbsolute(0.0f, 0.05f, 0.0f);
		if (InputHandler::get().isKeyHeld(GLFW_KEY_S))
			line->transform->translateAbsolute(0.0f, -0.05f, 0.0f);
		if (InputHandler::get().isKeyHeld(GLFW_KEY_D))
			line->transform->translateAbsolute(0.05f, 0.0f, 0.0f);
		if (InputHandler::get().isKeyHeld(GLFW_KEY_LEFT))
			line->transform->rotateAbsolute(2.0f, 0.0f, 0.0f, 1.0f);
		if (InputHandler::get().isKeyHeld(GLFW_KEY_RIGHT))
			line->transform->rotateAbsolute(-2.0f, 0.0f, 0.0f, 1.0f);
	};
};

int main() {
	// Create engine instance
	Game game = Game();

	// Initialize the engine
	if (!game.Initialize(1024, 768, "GnomeEngine Demo")) {
		std::cerr << "Failed to initialize GnomeEngine!" << std::endl;
		return -1;
	}

	// Run all the setup code.
	game.setup();

	// Run the engine (this will block until the window is closed)
	game.Run();

	// Engine will automatically shut down when destroyed.
	return 0;
}
