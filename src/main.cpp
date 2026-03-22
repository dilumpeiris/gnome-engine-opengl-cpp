#include <iostream>
#include <unistd.h>
#include <cmath>

// Not using this order give a redefinition error.
#include "core/GnomeEngine.h"
#include "input/InputHandler.h"
#include "entities/GRectangle.h"
#include <string>

class Game : public Gnome::GnomeEngine {
  public:
	// Create a Rectangle object using the prebuilt GRect Entity class.
	GRect *my_rect;

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
		my_rect = new GRect(0, 0, 512, 384);

		// Add my_rect to the game object manager in Gnome.
		this->addEntity(my_rect);

		// my_rect->material->addTexture("walking/walking-left-1.png", "walking-left");

		my_rect->animation->addAnimation("walking-left");

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
	}
	void Render() override {

		if (InputHandler::get().isKeyHeld(GLFW_KEY_A)) {
			my_rect->transform->translateAbsolute(-0.05f, 0.0f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_W)) {
			my_rect->transform->translateAbsolute(0.0f, 0.05f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_S)) {
			my_rect->transform->translateAbsolute(0.0f, -0.05f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_D)) {
			my_rect->transform->translateAbsolute(0.05f, 0.0f, 0.0f);
		}
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
