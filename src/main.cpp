#include <iostream>
#include <unistd.h>

#include "GnomeEngine.h"
#include "GRectangle.h"

class Game : public Gnome::GnomeEngine {
  public:
	// Create a Rectangle object using the prebuilt GRect Entity class.
	GRect *my_rect;

  public:
	void setup() {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		std::cout << "Working directory: " << cwd << std::endl;

		// Initialize my_rect with position and size data.
		my_rect = new GRect(0, 100, 512, 384);

		// Add my_rect to the game object manager in Gnome.
		Gnome::manager->addEntity(my_rect);

		// Add a texture to my_rect with the Material Component.
		my_rect->material->addTexture("crate.jpg");
	}

	void Render() override { my_rect->transform->translate(0.005f, 0.0f, 0.0f); }
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
