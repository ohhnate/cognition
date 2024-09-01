# Cognition Engine

Cognition Engine is a lightweight, modular game engine written in C, designed to provide a flexible foundation for 2D game development.

## Features

- Modular architecture for easy extension and customization
- SDL2-based rendering system
- Input management (keyboard, mouse, and gamepad support)
- Basic 2D physics system
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
│       │   ├── ai/
│       │   ├── audio/
│       │   ├── config/
│       │   ├── graphics/
│       │   ├── input/
│       │   ├── physics/
│       │   ├── resource/
│       │   ├── scripting/
│       │   └── utils/
│       ├── cognition.c
│       ├── cognition.h
│       ├── entity.c
│       ├── entity.h
│       ├── events.c
│       └── events.h
├── tests/
├── .gitattributes
├── .gitignore
└── CMakeLists.txt
```

## Building the Engine

Cognition Engine uses CMake as its build system. To build the engine:

1. Make sure you have CMake installed on your system.
2. Navigate to the project root directory.
3. Create a build directory: `mkdir build && cd build`
4. Run CMake: `cmake ..`
5. Build the project: `cmake --build .`

## Usage

(TODO: Add brief instructions on how to use the engine in a project)

## Dependencies

- SDL2
- SDL2_image

## Contributing

(TODO: Add information about how others can contribute to the project)

## License

(TODO: Add license information)

## Contact

(TODO: Add contact information or links to project resources)