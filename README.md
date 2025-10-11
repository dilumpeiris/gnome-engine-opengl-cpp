
# A Sample Usage (Current stable production)
```main.cpp``` to create a simple rectangle with a texture and move it across the screen.
```c++
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
```

![sample-rectangle-movement](https://github.com/user-attachments/assets/ed63e0e5-fd91-4982-8686-74059a34c1bc)



# What Is GnomeEngine?

GnomeEngine is a simple 2D game engine built with OpenGL and C++ as a **header only library/Framework**.

## Features

- **Single header** - Just include `GnomeEngine.h` and the other required libraries.
- OpenGL 3.3 Core Profile rendering
- GLFW window management
- Cross-platform support (Windows, macOS, Linux)
- Clean, extensible architecture

## Dependencies

- **OpenGL** - For rendering
- **GLAD** - To handle function pointers.
- **GLFW** - For window management and input handling
- **CMake** - For build system

## Building

### Prerequisites

On macOS, you can install dependencies using Homebrew:

```bash
brew install glfw cmake
```

### Build Instructions

1. Clone or navigate to the project directory
2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
3. Generate build files with CMake:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   make
   ```
5. Run the engine:
   ```bash
   ./bin/GnomeEngine
   ```

## Project Structure

```
GnomeEngine/
├── CMakeLists.txt          # Main CMake configuration
├── README.md               # This file
├── include/                # Header files
│   └── GnomeEngine.h       # Single header library (complete implementation)
└── src/                    # Source files
    └── main.cpp            # Demo application entry point
```

## Usage

Since GnomeEngine is a single header library, you just need to include the header file (with the required dependencies installed):

```cpp
#include "GnomeEngine.h"  // That's it! Everything is included

int main() {
    Gnome::GnomeEngine engine;
    
    if (!engine.Initialize(800, 600, "My Game")) {
        return -1;
    }
    
    engine.Run();
    return 0;
}
```

### Using GnomeEngine in Your Own Project

To use GnomeEngine in your own project:

1. Copy `include/GnomeEngine.h` to your project
2. Include it in your source files: `#include "GnomeEngine.h"`
3. Link against OpenGL and GLFW in your build system

## Controls

- **ESC** - Close the application

## Next Steps

- Shader management
- Mesh and model loading
- Texture management
- Scene management
- Input system
- Audio system
- Physics integration
- And much more!
