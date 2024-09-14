#ifndef COGNITION_H
#define COGNITION_H

#include <SDL.h>
#include "ecs/cog_ecs.h"
#include "input/cog_input.h"
#include "physics/cog_physics.h"
#include "resource/cog_resource.h"
#include "camera/cog_camera.h"
#include "audio/cog_audio.h"

typedef struct Renderer Renderer;

typedef struct CognitionEngine {
    SDL_Window* window;
    Renderer* renderer;
    ResourceManager* resource_manager;
    InputManager* input_manager;
    PhysicsWorld* physics_world;
    ECS* ecs;
    Camera camera;
    AudioSystem* audio_system;
    int is_running;
} CognitionEngine;

// Engine lifecycle functions
int cognition_init(CognitionEngine* engine, const char* title, int width, int height);
void cognition_run(CognitionEngine* engine);
void cognition_shutdown(CognitionEngine* engine);

#endif // COGNITION_H