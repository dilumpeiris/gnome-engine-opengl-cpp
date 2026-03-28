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

		// Initialize my_rect with position and size data.
		my_rect = new GRect(0, 0, 100, 100);

		// Add my_rect to the game object manager in Gnome.
		this->addEntity(my_rect);

		my_rect->material->addTexture("crate.jpg", "crate");
		my_rect->setColor(1.0f, 1.0f, 1.0f, 1.0f);

		my_rect->animation->addAnimation("walking-left", 50);

		my_rect->animation->addFrame("walking-left", "walking/walking-left-1.png", 0);
		my_rect->animation->addFrame("walking-left", "walking/walking-left-2.png", 1);
		my_rect->animation->addFrame("walking-left", "walking/walking-left-3.png", 2);
		my_rect->animation->addFrame("walking-left", "walking/walking-left-4.png", 3);
		my_rect->animation->addFrame("walking-left", "walking/walking-left-5.png", 4);
		my_rect->animation->addFrame("walking-left", "walking/walking-left-6.png", 5);
		my_rect->animation->addFrame("walking-left", "walking/walking-left-7.png", 6);
		my_rect->animation->addFrame("walking-left", "walking/walking-left-8.png", 7);

		// my_rect->material->useTexture("walking-left");

		my_rect->animation->playAnimation("walking-left");

		// Optionally set animation speed
		setAnimationSpeed(20.0f);

		// Initialize line
		line = new GLine();
		this->addEntity(line);
		line->setColor(0.0f, 1.0f, 0.0f, 1.0f); // Green line
	}
	void Render() override {

		if (InputHandler::get().isKeyHeld(GLFW_KEY_A)) {
			line->transform->translateAbsolute(-0.05f, 0.0f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_W)) {
			line->transform->translateAbsolute(0.0f, 0.05f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_S)) {
			line->transform->translateAbsolute(0.0f, -0.05f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_D)) {
			line->transform->translateAbsolute(0.05f, 0.0f, 0.0f);
		}

		if (InputHandler::get().isKeyHeld(GLFW_KEY_LEFT)) {
			line->transform->rotateAbsolute(2.0f, 0.0f, 0.0f, 1.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_RIGHT)) {
			line->transform->rotateAbsolute(-2.0f, 0.0f, 0.0f, 1.0f);
		}

		// Endpoints for collision system
		std::pair<glm::vec2, glm::vec2> endpoints = line->getLineEndpoints();
		// std::cout << "Line points: (" << endpoints.first.x << ", " << endpoints.first.y << ") to
		// ("
		//           << endpoints.second.x << ", " << endpoints.second.y << ")" << std::endl;
	}
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
