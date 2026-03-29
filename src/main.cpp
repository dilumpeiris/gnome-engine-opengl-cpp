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
	GRect *my_rect;

	void setup() {

		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		std::cout << "Working directory: " << cwd << std::endl;

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

		// Shader Passes are not handled here but for now this works.<D-s>
		my_rect->material->materials[0].addShaderPass("checker", GRectShader::vertexSrc, frag);

		TextureAsset texture;
		texture.filePath = "crate.jpg";

		TextureAsset texture2;
		texture2.filePath = "crate.jpg";

		TextureAsset texture3;
		texture3.filePath = "crate.jpg";

		AnimationAsset anim;
		anim.frameDuration = 100; // 100 ms per frame
		anim.loop = true;
		anim.width = 256;
		anim.height = 256;
		anim.depth = 3; // 3 frames in the animation

		anim.frames.push_back(texture);
		anim.frames.push_back(texture2);
		anim.frames.push_back(texture3);

		my_rect->animation->addAnimation("spin", anim);
	}

	void Render() override {};
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
