#ifndef COGNITION_H
#define COGNITION_H

#include <SDL.h>
#include "entity.h"
#include "events.h"
#include "modules/input/input_manager.h"
#include "modules/physics/physics.h"
#include "modules/resource/resource_manager.h"
#include "modules/graphics/renderer.h"

typedef struct CognitionEngine {
    SDL_Window* window;
    Renderer* renderer;
    ResourceManager* resource_manager;
    int is_running;
    EntityManager* entity_manager;
    EventSystem* event_system;
    InputManager* input_manager;
    PhysicsWorld* physics_world;
} CognitionEngine;

// Function declarations
int cognition_init(CognitionEngine* engine, const char* title, int width, int height);
void cognition_run(CognitionEngine* engine);
void cognition_shutdown(CognitionEngine* engine);

#endif // COGNITION_H