#ifndef COGNITION_H
#define COGNITION_H

#include <SDL.h>
#include "ecs.h"
#include "modules/input/input_manager.h"
#include "modules/physics/physics.h"
#include "modules/resource/resource_manager.h"

typedef struct Renderer Renderer;

typedef struct CognitionEngine {
    SDL_Window* window;
    Renderer* renderer;
    ResourceManager* resource_manager;
    InputManager* input_manager;
    PhysicsWorld* physics_world;
    ECS* ecs;
    int is_running;
} CognitionEngine;

int cognition_init(CognitionEngine* engine, const char* title, int width, int height);
void cognition_run(CognitionEngine* engine);
void cognition_shutdown(CognitionEngine* engine);

#endif // COGNITION_H