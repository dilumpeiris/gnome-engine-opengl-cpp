#include "GnomeEngine.h"
#include <iostream>
#include <unistd.h>

class Game :public Gnome::GnomeEngine {
 public:
	Gnome::GRectObject * rect2;
	Gnome::GRectObject * rect3;

  public:
	Game() {};

	void setup() {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		std::cout << "Working directory: " << cwd << std::endl;

		rect2 = new Gnome::GRectObject(0, 100, 512, 384);
		rect3 = new Gnome::GRectObject(0, 0, 200, 200);
		Gnome::GRectObject * rect4 = new Gnome::GRectObject(600, 0, 200, 500);


		Gnome::manager->addEntity(rect2);
		Gnome::manager->addEntity(rect3);
		Gnome::manager->addEntity(rect4);

 std::cout << "addr "<<  Gnome::manager->entities[0] << " " << rect2 << std::endl;



	}

	void Render() override {
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	for (const auto& entity : Gnome::manager->entities) {
    //		Entity* e = entity.get(); // raw pointer, just for access
    //		e->draw();
	//		}

		rect3->draw();
		rect2->draw();
		Gnome::manager->entities[2]->draw();
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
