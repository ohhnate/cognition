#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "core/cognition.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include "modules/graphics/renderer.h"
#include "modules/graphics/render_system.h"
#include "modules/resource/resource_manager.h"
#include "modules/input/input_manager.h"
#include "modules/physics/physics.h"
#include "modules/physics/transform_system.h"
#include "modules/physics/physics_system.h"
#include "modules/physics/physics_components.h"
#include "modules/graphics/render_components.h"

static int init_sdl_and_window(CognitionEngine* engine, const char* title, int width, int height) {
    cog_log_info("Initializing SDL and creating window...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        cog_log_error("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 0;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cog_log_error("SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
        SDL_Quit();
        return 0;
    }

    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (engine->window == NULL) {
        cog_log_error("Window could not be created! SDL_Error: %s", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 0;
    }

    cog_log_info("SDL initialized and window created successfully.");
    return 1;
}

static int init_renderer(CognitionEngine* engine) {
    cog_log_info("Initializing renderer...");

    engine->renderer = COG_NEW(Renderer);
    if (!engine->renderer) {
        cog_log_error("Failed to allocate memory for renderer");
        return 0;
    }

    if (!renderer_init(engine->renderer, engine->window)) {
        cog_log_error("Renderer could not be initialized!");
        COG_DELETE(engine->renderer);
        return 0;
    }

    cog_log_info("Renderer initialized successfully.");
    return 1;
}

static int init_resource_manager(CognitionEngine* engine) {
    cog_log_info("Initializing resource manager...");

    engine->resource_manager = COG_NEW(ResourceManager);
    if (!engine->resource_manager) {
        cog_log_error("Failed to allocate memory for resource manager");
        return 0;
    }

    resource_manager_init(engine->resource_manager, engine->renderer->sdl_renderer);

    if (!resource_manager_load_image(engine->resource_manager, "test_sprite", "../assets/test_sprite.png")) {
        cog_log_error("Failed to load test sprite!");
        return 0;
    }

    cog_log_info("Resource manager initialized and test sprite loaded successfully.");
    return 1;
}

static int init_subsystems(CognitionEngine* engine) {
    cog_log_info("Initializing subsystems...");

    engine->input_manager = COG_NEW(InputManager);
    if (!engine->input_manager) {
        cog_log_error("Failed to allocate memory for input manager");
        return 0;
    }
    input_manager_init(engine->input_manager);

    engine->physics_world = COG_NEW(PhysicsWorld);
    if (!engine->physics_world) {
        cog_log_error("Failed to allocate memory for physics world");
        return 0;
    }
    physics_world_init(engine->physics_world);

    cog_log_info("Subsystems initialized successfully.");
    return 1;
}

static int init_ecs(CognitionEngine* engine) {
    cog_log_info("Initializing ECS...");

    engine->ecs = COG_NEW(ECS);
    if (!engine->ecs) {
        cog_log_error("Failed to allocate memory for ECS");
        return 0;
    }

    if (ecs_init(engine->ecs) != ECS_OK) {
        cog_log_error("Failed to initialize ECS");
        return 0;
    }

    System transform_sys = {transform_system, {COMPONENT_TRANSFORM}, 1};
    System physics_sys = {physics_system, {COMPONENT_TRANSFORM, COMPONENT_PHYSICS}, 2};
    System render_sys = {render_system, {COMPONENT_TRANSFORM, COMPONENT_RENDER}, 2};

    add_system(engine->ecs, transform_sys);
    add_system(engine->ecs, physics_sys);
    add_system(engine->ecs, render_sys);

    cog_log_info("ECS initialized and systems added successfully.");
    return 1;
}

static int create_test_entity(CognitionEngine* engine) {
    cog_log_info("Creating test entity...");

    Entity* test_entity = NULL;
    ECSResult result = create_entity(engine->ecs, &test_entity);
    if (result != ECS_OK || test_entity == NULL) {
        cog_log_error("Failed to create test entity! Error code: %d", result);
        return 0;
    }

    Component transform_component = create_transform_component((Vector2){100, 100}, 0, (Vector2){1, 1});
    result = add_component(engine->ecs, test_entity, transform_component);
    if (result != ECS_OK) {
        cog_log_error("Failed to add transform component! Error code: %d", result);
        return 0;
    }

    Component render_component = create_render_component("test_sprite");
    result = add_component(engine->ecs, test_entity, render_component);
    if (result != ECS_OK) {
        cog_log_error("Failed to add render component! Error code: %d", result);
        return 0;
    }

    cog_log_info("Test entity created successfully with transform and render components.");
    return 1;
}

int cognition_init(CognitionEngine* engine, const char* title, int width, int height) {
    cog_log_info("Initializing Cognition Engine...");

    if (!init_sdl_and_window(engine, title, width, height)) return 0;
    if (!init_renderer(engine)) return 0;
    if (!init_resource_manager(engine)) return 0;
    if (!init_subsystems(engine)) return 0;
    if (!init_ecs(engine)) return 0;
    if (!create_test_entity(engine)) return 0;

    engine->is_running = 1;
    cog_log_info("Cognition Engine initialized successfully.");
    return 1;
}

static void handle_events(CognitionEngine* engine) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            engine->is_running = 0;
            cog_log_info("Quit event received");
        }
        input_manager_handle_event(engine->input_manager, &e);
    }
}

static void update_game_state(CognitionEngine* engine) {
    input_manager_update(engine->input_manager);
    
    ECSResult result = ecs_update(engine->ecs, engine);
    if (result != ECS_OK) {
        cog_log_error("ECS update failed with error code: %d", result);
        engine->is_running = 0;
    }
}

static void render_frame(CognitionEngine* engine) {
    SDL_Color clear_color = {0, 0, 0, 255}; // Black
    clear(engine->renderer, clear_color);

    render_entities(engine->renderer, engine->ecs, engine);

    present(engine->renderer);
}

void cognition_run(CognitionEngine* engine) {
    cog_log_info("Entering main game loop");
    int frame_count = 0;

    while (engine->is_running) {
        cog_log_debug("Frame %d start", frame_count);

        handle_events(engine);
        update_game_state(engine);
        render_frame(engine);

        frame_count++;
        SDL_Delay(16);  // Roughly 60 FPS

        cog_log_debug("Frame %d end. is_running: %d", frame_count, engine->is_running);
    }

    cog_log_info("Exiting main game loop");
}

static void cleanup_subsystems(CognitionEngine* engine) {
    if (engine->ecs) {
        ecs_cleanup(engine->ecs);
        COG_DELETE(engine->ecs);
    }

    if (engine->resource_manager) {
        resource_manager_cleanup(engine->resource_manager);
        COG_DELETE(engine->resource_manager);
    }

    if (engine->renderer) {
        renderer_cleanup(engine->renderer);
        COG_DELETE(engine->renderer);
    }

    if (engine->input_manager) {
        input_manager_cleanup(engine->input_manager);
        COG_DELETE(engine->input_manager);
    }

    if (engine->physics_world) {
        physics_world_cleanup(engine->physics_world);
        COG_DELETE(engine->physics_world);
    }
}

void cognition_shutdown(CognitionEngine* engine) {
    cog_log_info("Shutting down Cognition Engine...");

    cleanup_subsystems(engine);

    if (engine->window) {
        SDL_DestroyWindow(engine->window);
    }

    IMG_Quit();
    SDL_Quit();

    cog_log_info("Cognition Engine shut down successfully.");
}