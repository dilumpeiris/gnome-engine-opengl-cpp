#include "GnomeEngine.h"
#include <iostream>
#include <unistd.h>

// TODO: Make transform.translate() available from Render() {later Update()}.
//
class Game : public Gnome::GnomeEngine {
  public:
	Gnome::GRect *rect2;
	Gnome::GRect *rect3;

  public:
	Game() {};

	void setup() {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		std::cout << "Working directory: " << cwd << std::endl;

		rect2 = new Gnome::GRect(0, 100, 512, 384);
		rect3 = new Gnome::GRect(0, 0, 200, 200);
		Gnome::GRect *rect4 = new Gnome::GRect(600, 0, 200, 500);

		Gnome::manager->addEntity(rect2);
		Gnome::manager->addEntity(rect3);
		Gnome::manager->addEntity(rect4);
	}

	void Render() override {

		rect2->transform->translate(0.005f, 0.0f, 0.0f);
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
	game.setup();
	// Run the engine (this will block until the window is closed)
	game.Run();

	// Engine will automatically shut down when destroyed
	return 0;
}
