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
	float animationSpeedFPS = 12.0f; // Default animation speed

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

		// for (int i = 1; i <= 8; i++) {
		// 	my_rect->material->addTexture(
		// 	    ("walking/walking-left-" + std::to_string(i) + ".png").c_str(),
		// 	    ("walking-left-" + std::to_string(i)).c_str());
		// }

		my_rect->material->addTexture("walking/walking-left-1.png", "walking-left-1");
		my_rect->material->addTexture("walking/walking-left-2.png", "walking-left-2");
		my_rect->material->addTexture("walking/walking-left-3.png", "walking-left-3");
		my_rect->material->addTexture("walking/walking-left-4.png", "walking-left-4");
		my_rect->material->addTexture("walking/walking-left-5.png", "walking-left-5");
		my_rect->material->addTexture("walking/walking-left-6.png", "walking-left-6");
		my_rect->material->addTexture("walking/walking-left-7.png", "walking-left-7");
		my_rect->material->addTexture("walking/walking-left-8.png", "walking-left-8");

		my_rect->material->setCurrentTextureLocation(0);

		// Optionally set animation speed
		setAnimationSpeed(10.0f);
	}
	void Render() override {

		if (InputHandler::get().isKeyHeld(GLFW_KEY_A)) {
			my_rect->transform->translateAbsolute(-0.005f, 0.0f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_W)) {
			my_rect->transform->translateAbsolute(0.0f, 0.005f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_S)) {
			my_rect->transform->translateAbsolute(0.0f, -0.005f, 0.0f);
		}
		if (InputHandler::get().isKeyHeld(GLFW_KEY_D)) {
			my_rect->transform->translateAbsolute(0.005f, 0.0f, 0.0f);
		}

		if (InputHandler::get().isMouseHeld(GLFW_MOUSE_BUTTON_1)) {
			double dx = InputHandler::get().mouseDeltaX;
			double dy = InputHandler::get().mouseDeltaY;

			if (dx != 0.0 || dy != 0.0) {
				if (std::abs(dx) >= std::abs(dy)) {
					my_rect->transform->rotateAbsolute(5.0f, 0.0f, dx, 0.0f);
				} else {
					my_rect->transform->rotateAbsolute(5.0f, dy, 0.0f, 0.0f);
				}
			}
		}

		if (InputHandler::get().isKeyHeld(GLFW_KEY_SPACE)) {
			animationTimer += getDeltaTime();
			if (animationTimer >= 1.0f / animationSpeedFPS) {
				currentAnimationFrame++;
				if (currentAnimationFrame >= maxAnimationFrames) {
					currentAnimationFrame = 0;
				}
				my_rect->material->setCurrentTextureLocation(currentAnimationFrame);
				animationTimer = 0.0f;
			}
		} else {
			// Optional: Reset timer/frame when not pressing space
			// animationTimer = 0.0f;
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
