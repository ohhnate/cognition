#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "cognition.h"
#include "modules/graphics/renderer.h"
#include "modules/graphics/render_system.h"
#include "modules/resource/resource_manager.h"
#include "modules/input/input_manager.h"
#include "modules/physics/physics.h"
#include "modules/physics/transform_system.h"
#include "modules/physics/physics_system.h"
#include "modules/physics/physics_components.h"
#include "modules/graphics/render_components.h"

int cognition_init(CognitionEngine* engine, const char* title, int width, int height) {
    printf("Initializing Cognition Engine...\n");

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    printf("SDL initialized successfully.\n");

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 0;
    }
    printf("SDL_image initialized successfully.\n");

    // Create window
    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (engine->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 0;
    }
    printf("Window created successfully.\n");

    // Initialize renderer
    engine->renderer = malloc(sizeof(Renderer));
    if (!renderer_init(engine->renderer, engine->window)) {
        printf("Renderer could not be initialized!\n");
        SDL_DestroyWindow(engine->window);
        IMG_Quit();
        SDL_Quit();
        return 0;
    }
    printf("Renderer initialized successfully.\n");

    // Initialize resource manager
    engine->resource_manager = malloc(sizeof(ResourceManager));
    resource_manager_init(engine->resource_manager, engine->renderer->sdl_renderer);
    printf("Resource manager initialized.\n");

    // Load test sprite
    if (!resource_manager_load_image(engine->resource_manager, "test_sprite", "../assets/test_sprite.png")) {
        printf("Failed to load test sprite!\n");
        renderer_cleanup(engine->renderer);
        free(engine->renderer);
        SDL_DestroyWindow(engine->window);
        IMG_Quit();
        SDL_Quit();
        return 0;
    }
    printf("Successfully loaded test sprite.\n");

    // Initialize input manager
    engine->input_manager = malloc(sizeof(InputManager));
    input_manager_init(engine->input_manager);
    printf("Input manager initialized.\n");

    // Initialize physics world
    engine->physics_world = malloc(sizeof(PhysicsWorld));
    physics_world_init(engine->physics_world);
    printf("Physics world initialized.\n");

    // Initialize ECS
    engine->ecs = malloc(sizeof(ECS));
    ecs_init(engine->ecs);
    printf("ECS initialized.\n");

    // Initialize ECS systems
    System transform_sys = {transform_system, {COMPONENT_TRANSFORM}, 1};
    System physics_sys = {physics_system, {COMPONENT_TRANSFORM, COMPONENT_PHYSICS}, 2};
    System render_sys = {render_system, {COMPONENT_TRANSFORM, COMPONENT_RENDER}, 2};

    add_system(engine->ecs, transform_sys);
    add_system(engine->ecs, physics_sys);
    add_system(engine->ecs, render_sys);
    printf("ECS systems added.\n");

    // Create a test entity
    Entity* test_entity = NULL;
    ECSResult result = create_entity(engine->ecs, &test_entity);
    if (result != ECS_OK || test_entity == NULL) {
        printf("Failed to create test entity! Error code: %d\n", result);
        cognition_shutdown(engine);
        return 0;
    }

    // Add transform component
    Component transform_component = create_transform_component((Vector2){100, 100}, 0, (Vector2){1, 1});
    result = add_component(engine->ecs, test_entity, transform_component);
    if (result != ECS_OK) {
        printf("Failed to add transform component! Error code: %d\n", result);
        cognition_shutdown(engine);
        return 0;
    }

    // // Add physics component
    // Vector2D initial_position = {100, 100};
    // Vector2D initial_velocity = {0, 0};
    // Vector2D initial_acceleration = {0, 9.8f};
    // Component physics_component = create_physics_component(initial_position, initial_velocity, initial_acceleration, 1.0f, false);
    // result = ecs_add_component(engine->ecs, test_entity, physics_component);
    // if (result != ECS_OK) {
    //     printf("Failed to add physics component! Error code: %d\n", result);
    //     cognition_shutdown(engine);
    //     return 0;
    // }

    // Add render component
    Component render_component = create_render_component("test_sprite");
    result = add_component(engine->ecs, test_entity, render_component);
    if (result != ECS_OK) {
        printf("Failed to add render component! Error code: %d\n", result);
        cognition_shutdown(engine);
        return 0;
    }

    printf("Created test entity with transform and render components.\n");

    // Set running flag
    engine->is_running = 1;
    printf("Engine is_running set to %d\n", engine->is_running);

    printf("Cognition Engine initialized successfully.\n");
    return 1;
}

void cognition_run(CognitionEngine* engine) {
    SDL_Event e;
    int frame_count = 0;
    printf("Entering main loop\n");
    while (engine->is_running) {
        printf("Frame %d start\n", frame_count);

        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                engine->is_running = 0;
                printf("Quit event received\n");
            }
            input_manager_handle_event(engine->input_manager, &e);
        }
       
        // Update input
        input_manager_update(engine->input_manager);
        printf("Input updated\n");
       
        // Update ECS
        ECSResult result = ecs_update(engine->ecs, engine);
        if (result != ECS_OK) {
            printf("ECS update failed with error code: %d\n", result);
            engine->is_running = 0;
            break;
        }
        printf("ECS updated\n");
       
        // Render
        SDL_Color clear_color = {0, 0, 0, 255}; // Black
        clear(engine->renderer, clear_color);
        printf("Renderer cleared\n");

        // Render ECS entities
        render_entities(engine->renderer, engine->ecs, engine);
        printf("Entities rendered\n");

        present(engine->renderer);
        printf("Renderer presented\n");

        frame_count++;
        
        SDL_Delay(16);  // Roughly 60 FPS

        printf("Frame %d end. is_running: %d\n", frame_count, engine->is_running);
    }
    printf("Exiting main loop\n");
}

void cognition_shutdown(CognitionEngine* engine) {
    ecs_cleanup(engine->ecs);
    free(engine->ecs);
    resource_manager_cleanup(engine->resource_manager);
    free(engine->resource_manager);
    renderer_cleanup(engine->renderer);
    free(engine->renderer);
    input_manager_cleanup(engine->input_manager);
    free(engine->input_manager);
    physics_world_cleanup(engine->physics_world);
    free(engine->physics_world);
    SDL_DestroyWindow(engine->window);
    IMG_Quit();
    SDL_Quit();
}