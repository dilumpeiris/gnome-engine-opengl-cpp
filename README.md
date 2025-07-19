# GnomeEngine

A simple game engine built with OpenGL and C++ as a **single header library**.

## Features

- **Single header library** - Just include `GnomeEngine.h` and you're ready to go!
- OpenGL 3.3 Core Profile rendering
- GLFW window management
- Cross-platform support (Windows, macOS, Linux)
- Modern C++17 codebase
- Clean, extensible architecture

## Dependencies

- **OpenGL** - For rendering
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

Since GnomeEngine is a single header library, you just need to include the header file:

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
4. That's it! No separate compilation needed.

## Controls

- **ESC** - Close the application

## Next Steps

This is a basic boilerplate. You can extend it by adding:

- Shader management
- Mesh and model loading
- Texture management
- Scene management
- Input system
- Audio system
- Physics integration
- And much more!

## License

MIT License
