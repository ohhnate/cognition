#include <stdio.h>
#include "core/modules/graphics/renderer.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include "core/cognition.h"
#include "render_components.h"
#include "core/modules/physics/transform_components.h"
#include "core/modules/resource/resource_manager.h"
#include "sprite.h"

int renderer_init(Renderer* renderer, SDL_Window* window) {
    renderer->sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer->sdl_renderer == NULL) {
        cog_log_error("Renderer could not be created! SDL Error: %s", SDL_GetError());
        return 0;
    }
    cog_log_info("Renderer initialized successfully");
    return 1;
}

void clear(Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer->sdl_renderer);
}

void present(Renderer* renderer) {
    SDL_RenderPresent(renderer->sdl_renderer);
}

void draw_rect(Renderer* renderer, SDL_Rect* rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer->sdl_renderer, rect);
}

void draw_line(Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer->sdl_renderer, x1, y1, x2, y2);
}

void render_entities(Renderer* renderer, ECS* ecs, CognitionEngine* engine) {
    printf("Rendering entities. Entity count: %d\n", ecs->entity_count);
    for (int i = 0; i < ecs->entity_count; i++) {
        Entity* entity = &ecs->entities[i];
        Component* render_component = NULL;
        Component* transform_component = NULL;

        ECSResult render_result = get_component(ecs, entity, COMPONENT_RENDER, &render_component);
        ECSResult transform_result = get_component(ecs, entity, COMPONENT_TRANSFORM, &transform_component);

        if (render_result == ECS_OK && transform_result == ECS_OK && render_component && transform_component) {
            RenderComponentData* render_data = (RenderComponentData*)render_component->data;
            Transform* transform = (Transform*)transform_component->data;

            printf("Rendering entity %d. Sprite ID: %s, Position: (%f, %f)\n", 
                   entity->id, render_data->sprite_id, transform->position.x, transform->position.y);

            Sprite* sprite = resource_manager_get_sprite(engine->resource_manager, render_data->sprite_id);
            if (sprite) {
                sprite_render(sprite, renderer->sdl_renderer, 
                              (int)transform->position.x, 
                              (int)transform->position.y);
            } else {
                printf("Failed to get sprite for entity %d\n", entity->id);
            }
        }
    }
}

void renderer_cleanup(Renderer* renderer) {
    if (renderer->sdl_renderer) {
        SDL_DestroyRenderer(renderer->sdl_renderer);
        renderer->sdl_renderer = NULL;
    }
    cog_log_info("Renderer cleaned up");
}