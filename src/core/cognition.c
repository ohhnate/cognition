#include "cognition.h"
#include <stdio.h>
#include <SDL_image.h>
#include <stdlib.h>

int cognition_init(CognitionEngine* engine, const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
   
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 0;
    }
   
    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (engine->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
   
    engine->renderer = malloc(sizeof(Renderer));
    if (!renderer_init(engine->renderer, engine->window)) {
        return 0;
    }
   
    engine->resource_manager = malloc(sizeof(ResourceManager));
    resource_manager_init(engine->resource_manager, engine->renderer->sdl_renderer);
   
    // Load test sprite
    if (!resource_manager_load_image(engine->resource_manager, "test_sprite", "../assets/test_sprite.png")) {
        printf("Failed to load test sprite!\n");
        // Handle error or continue
    }
   
    engine->is_running = 1;
    engine->entity_manager = malloc(sizeof(EntityManager));
    entity_manager_init(engine->entity_manager);
    engine->event_system = malloc(sizeof(EventSystem));
    event_system_init(engine->event_system);
    engine->input_manager = malloc(sizeof(InputManager));
    input_manager_init(engine->input_manager);
    engine->physics_world = malloc(sizeof(PhysicsWorld));
    physics_world_init(engine->physics_world);
    return 1;
}

void cognition_run(CognitionEngine* engine) {
    SDL_Event e;
    while (engine->is_running) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                engine->is_running = 0;
            }
            event_system_dispatch(engine->event_system, &e);
            input_manager_handle_event(engine->input_manager, &e);
        }
       
        // Update input
        input_manager_update(engine->input_manager);
       
        // Update physics
        physics_world_update(engine->physics_world);
       
        // Update game state
        entity_manager_update(engine->entity_manager);
       
        // Render
        SDL_Color clear_color = {255, 255, 255, 255}; // White
        renderer_clear(engine->renderer, clear_color);
       
        // Draw test sprite
        Sprite* test_sprite = resource_manager_get_sprite(engine->resource_manager, "test_sprite");
        if (test_sprite != NULL) {
            sprite_render(test_sprite, engine->renderer->sdl_renderer, 100, 100);
        }
       
        entity_manager_render(engine->entity_manager, engine->renderer->sdl_renderer);
        renderer_present(engine->renderer);
    }
}

void cognition_shutdown(CognitionEngine* engine) {
    entity_manager_shutdown(engine->entity_manager);
    free(engine->entity_manager);
    event_system_shutdown(engine->event_system);
    free(engine->event_system);
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