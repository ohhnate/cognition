#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "cognition.h"
#include "utils/cog_memory.h"
#include "utils/cog_error.h"
#include "ecs/cog_ecs.h"
#include "resource/cog_resource.h"
#include "input/cog_input.h"
#include "graphics/cog_renderer.h"
#include "graphics/cog_render_system.h"
#include "graphics/cog_render_components.h"
#include "physics/cog_physics.h"
#include "physics/cog_physics_system.h"
#include "physics/cog_physics_components.h"
#include "camera/cog_camera.h"

static_assert(sizeof(Component) > 0, "Component type is not properly defined");

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

    engine->resource_manager = resource_manager_create(engine->renderer->sdl_renderer);
    if (!engine->resource_manager) {
        cog_log_error("Failed to create resource manager");
        return 0;
    }
    cog_log_debug("Resource manager created successfully");

    const char* texture_path = "../assets/test_sprite.png";
    Texture* texture = resource_manager_load_texture(engine->resource_manager, "test_sprite", texture_path);
    if (!texture) {
        cog_log_error("Failed to load test sprite from path: %s", texture_path);
        return 0;
    }
    cog_log_debug("Test sprite loaded successfully from path: %s", texture_path);

    // Verify that the texture can be retrieved
    if (!resource_manager_get_texture(engine->resource_manager, "test_sprite")) {
        cog_log_error("Failed to retrieve loaded test sprite");
        return 0;
    }
    cog_log_debug("Test sprite retrieved successfully");

    cog_log_info("Resource manager initialized and test sprite loaded successfully.");
    return 1;
}

static int init_audio_system(CognitionEngine* engine) {
    cog_log_info("Initializing audio system...");

    engine->audio_system = COG_NEW(AudioSystem);
    if (!engine->audio_system) {
        cog_log_error("Failed to allocate memory for audio system");
        return 0;
    }

    if (!audio_system_init(engine->audio_system)) {
        cog_log_error("Failed to initialize audio system");
        return 0;
    }

    cog_log_info("Audio system initialized successfully.");
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

    if (cog_ecs_init(engine->ecs) != ECS_OK) {
        cog_log_error("Failed to initialize ECS");
        return 0;
    }

    System physics_sys = {physics_system, {COMPONENT_PHYSICS}, 1};
    System render_sys = {render_system, {COMPONENT_RENDER}, 1};

    cog_add_system(engine->ecs, physics_sys);
    cog_add_system(engine->ecs, render_sys);

    cog_log_info("ECS initialized and systems added successfully.");
    return 1;
}

static int create_test_entity(CognitionEngine* engine) {
    cog_log_info("Creating test entity...");

    Entity* test_entity = NULL;
    ECSResult result = cog_create_entity(engine->ecs, &test_entity);
    if (result != ECS_OK || test_entity == NULL) {
        cog_log_error("Failed to create test entity! Error code: %d", result);
        return 0;
    }
    cog_log_debug("Test entity created successfully");

    // Set initial transform values
    transform_set_position(&test_entity->transform, (Vector2){100, 100});
    transform_set_rotation(&test_entity->transform, 0);
    transform_set_scale(&test_entity->transform, (Vector2){1, 1});
    cog_log_debug("Transform set for test entity");

    // Create and add render component
    Texture* texture = resource_manager_get_texture(engine->resource_manager, "test_sprite");
    if (!texture) {
        cog_log_error("Failed to get texture for test entity");
        return 0;
    }
    cog_log_debug("Texture retrieved successfully");

    Sprite* sprite = sprite_create(texture);
    if (!sprite) {
        cog_log_error("Failed to create sprite for test entity");
        return 0;
    }
    cog_log_debug("Sprite created successfully");
    
    Component render_component = create_render_component(sprite, 0, 0);
    result = cog_add_component(engine->ecs, test_entity, render_component);
    if (result != ECS_OK) {
        cog_log_error("Failed to add render component! Error code: %d", result);
        return 0;
    }
    cog_log_debug("Render component added successfully");

    cog_log_info("Test entity created successfully with transform and render component.");
    return 1;
}

int cognition_init(CognitionEngine* engine, const char* title, int width, int height) {
    cog_log_info("Initializing Cognition Engine...");
    
    if (!init_sdl_and_window(engine, title, width, height)) {
        cog_log_error("Failed to initialize SDL and window");
        return 0;
    }
    cog_log_info("SDL and window initialized successfully");

    if (!init_renderer(engine)) {
        cog_log_error("Failed to initialize renderer");
        return 0;
    }
    cog_log_info("Renderer initialized successfully");

    renderer_begin_batch(engine->renderer);
    cog_log_info("Renderer batch begun");

    if (!init_resource_manager(engine)) {
        cog_log_error("Failed to initialize resource manager");
        return 0;
    }
    cog_log_info("Resource manager initialized successfully");

    if (!init_subsystems(engine)) {
        cog_log_error("Failed to initialize subsystems");
        return 0;
    }
    cog_log_info("Subsystems initialized successfully");

    if (!init_ecs(engine)) {
        cog_log_error("Failed to initialize ECS");
        return 0;
    }
    cog_log_info("ECS initialized successfully");

    if (!init_audio_system(engine)) {
        cog_log_error("Failed to initialize audio system");
        return 0;
    }
    cog_log_info("Audio system initialized successfully");

    if (!create_test_entity(engine)) {
        cog_log_error("Failed to create test entity");
        return 0;
    }
    cog_log_info("Test entity created successfully");
   
    camera_init(&engine->camera, width, height);
    cog_log_info("Camera initialized");

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

static void update_game_state(CognitionEngine* engine, float delta_time) {
    input_manager_update(engine->input_manager);
    cog_ecs_update(engine->ecs, engine, delta_time);
}

static void update_physics(CognitionEngine* engine, float fixed_delta_time) {
    physics_world_update(engine->physics_world, fixed_delta_time);
}

void render_frame(CognitionEngine* engine) {
    SDL_Color clear_color = {0, 0, 0, 255}; // Black
    clear(engine->renderer, clear_color);
    
    camera_apply(&engine->camera, engine->renderer->sdl_renderer);
    render_entities(engine->renderer, engine->ecs, engine);
    camera_revert(&engine->camera, engine->renderer->sdl_renderer);
    present(engine->renderer);
}

void cognition_run(CognitionEngine* engine) {
    int frame_count = 0;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    float delta_time = 0;

    const double FIXED_TIME_STEP = 1.0 / 60.0;  // 60 updates per second
    double accumulator = 0.0;

    while (engine->is_running) {
        last = now;
        now = SDL_GetPerformanceCounter();
        
        delta_time = (float)((now - last) / (double)SDL_GetPerformanceFrequency());
        accumulator += delta_time;

        // cog_log_debug("Frame %d start", frame_count);

        handle_events(engine);

        // Fixed time step update for physics
        while (accumulator >= FIXED_TIME_STEP) {
            update_physics(engine, (float)FIXED_TIME_STEP);
            accumulator -= FIXED_TIME_STEP;
        }

        update_game_state(engine, delta_time);
        camera_update(&engine->camera, delta_time);
        render_frame(engine);

        frame_count++;

        // cog_log_debug("Frame %d end. is_running: %d", frame_count, engine->is_running);
    }

    cog_log_info("Exiting main game loop");
}

static void cleanup_subsystems(CognitionEngine* engine) {
    if (engine->ecs) {
        cog_ecs_cleanup(engine->ecs);
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

    if (engine->audio_system) {
        audio_system_cleanup(engine->audio_system);
        COG_DELETE(engine->audio_system);
    }
}

void cognition_shutdown(CognitionEngine* engine) {
    cog_log_info("Shutting down Cognition Engine...");
    renderer_end_batch(engine->renderer);
    cleanup_subsystems(engine);
    if (engine->resource_manager) {
        resource_manager_destroy(engine->resource_manager);
    }
    if (engine->window) {
        SDL_DestroyWindow(engine->window);
    }
    IMG_Quit();
    SDL_Quit();
    cog_log_info("Cognition Engine shut down successfully.");
}