# Cognition Engine

Cognition Engine is a lightweight, modular game engine written in C, designed to provide a flexible foundation for 2D game development.

## Features

- Entity Component System (ECS) architecture for efficient game object management
- SDL2-based rendering system with sprite support
- Input management (keyboard, mouse, and gamepad support)
- 2D physics system with collision detection and resolution
- Resource management for efficient asset handling
- Event system for decoupled communication between engine components
- Configuration management system
- Utility functions for common mathematical operations

## Directory Structure

```
cognition/
├── assets/
├── build/
├── docs/
├── examples/
├── include/
├── lib/
├── SDL2/
├── SDL2_image/
├── src/
│   ├── main.c
│   └── core/
│       ├── modules/
│       │   ├── config/
│       │   ├── graphics/
│       │   ├── input/
│       │   ├── physics/
│       │   ├── resource/
│       │   └── utils/
│       ├── cognition.c
│       ├── cognition.h
│       ├── ecs.c
│       ├── ecs.h
│       ├── events.c
│       └── events.h
├── tests/
├── .gitattributes
├── .gitignore
└── CMakeLists.txt
```

## Building the Engine

Cognition Engine uses CMake as its build system. There are two ways to build the engine:

### Using the Build Script

For convenience, a batch file is provided to automate the build process:

1. Navigate to the project root directory in a terminal.
2. Run the batch file:
   - In Command Prompt: `build_and_run.bat`
   - In PowerShell: `.\build_and_run.bat`

This script will create a build directory, run CMake, build the project, and run the executable.

### Manual Build

If you prefer to build manually or if you're on a non-Windows system, follow these steps:

1. Make sure you have CMake installed on your system.
2. Navigate to the project root directory.
3. Create a build directory: `mkdir build`
4. Change to the build directory: `cd build`
5. Run CMake: `cmake ..`
6. Build the project: `cmake --build .`

After building, you can find the executable in the `build/Debug` directory (on Windows) or just the `build` directory (on Unix-like systems).

To run the game after building:
- On Windows: `Debug\cognition.exe` (from the `build` directory)
- On Unix-like systems: `./cognition` (from the `build` directory)

## Usage

To use Cognition Engine in your project:

1. Include the necessary headers from the `src/core` directory.
2. Initialize the engine using `cognition_init()`.
3. Create entities and add components using the ECS functions.
4. Implement your game logic using the provided systems or create custom ones.
5. Run the main game loop using `cognition_run()`.
6. Clean up resources with `cognition_shutdown()` when your game exits.

For more detailed usage instructions and examples refer to the documentation in the `docs/` directory.

## Dependencies

- SDL2
- SDL2_image

## Contributing

TODO: Add contributing requirements

## License

TODO: Add licensing information

## Contact

TODO: Add contact information