#include "cog_renderer.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include "core/cognition.h"
#include "cog_render_components.h"
#include "core/resource/cog_resource.h"
#include "cog_sprite.h"
#include <math.h>

int renderer_init(Renderer* renderer, SDL_Window* window) {
    renderer->sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer->sdl_renderer == NULL) {
        cog_log_error("Renderer could not be created! SDL Error: %s", SDL_GetError());
        return 0;
    }
    renderer->vertex_count = 0;
    renderer->current_texture = NULL;
    cog_log_info("Renderer initialized successfully");
    return 1;
}

void renderer_begin_batch(Renderer* renderer) {
    renderer->vertex_count = 0;
    renderer->current_texture = NULL;
}

void renderer_end_batch(Renderer* renderer) {
    renderer_flush(renderer);
}

void renderer_draw_sprite(Renderer* renderer, Texture* texture, SDL_Rect* src_rect, SDL_Rect* dest_rect, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    if (renderer->current_texture != texture || renderer->vertex_count >= MAX_BATCH_SIZE * 4) {
        renderer_flush(renderer);
        renderer->current_texture = texture;
    }

    float tex_w = (float)texture->width;
    float tex_h = (float)texture->height;

    SDL_Rect src = src_rect ? *src_rect : (SDL_Rect){0, 0, texture->width, texture->height};
    SDL_Rect dest = *dest_rect;

    float sin_angle = (float)sin(angle * M_PI / 180.0);
    float cos_angle = (float)cos(angle * M_PI / 180.0);

    SDL_FPoint corners[4] = {
        {0.0f, 0.0f},
        {(float)dest.w, 0.0f},
        {(float)dest.w, (float)dest.h},
        {0.0f, (float)dest.h}
    };

    SDL_FPoint center_point = center ? (SDL_FPoint){(float)center->x, (float)center->y} : (SDL_FPoint){dest.w / 2.0f, dest.h / 2.0f};

    for (int i = 0; i < 4; i++) {
        float x = corners[i].x - center_point.x;
        float y = corners[i].y - center_point.y;
        corners[i].x = x * cos_angle - y * sin_angle + center_point.x + dest.x;
        corners[i].y = x * sin_angle + y * cos_angle + center_point.y + dest.y;
    }

    float tex_coords[4][2] = {
        {src.x / tex_w, src.y / tex_h},
        {(src.x + src.w) / tex_w, src.y / tex_h},
        {(src.x + src.w) / tex_w, (src.y + src.h) / tex_h},
        {src.x / tex_w, (src.y + src.h) / tex_h}
    };

    if (flip & SDL_FLIP_HORIZONTAL) {
        float temp;
        temp = tex_coords[0][0]; tex_coords[0][0] = tex_coords[1][0]; tex_coords[1][0] = temp;
        temp = tex_coords[3][0]; tex_coords[3][0] = tex_coords[2][0]; tex_coords[2][0] = temp;
    }
    if (flip & SDL_FLIP_VERTICAL) {
        float temp;
        temp = tex_coords[0][1]; tex_coords[0][1] = tex_coords[3][1]; tex_coords[3][1] = temp;
        temp = tex_coords[1][1]; tex_coords[1][1] = tex_coords[2][1]; tex_coords[2][1] = temp;
    }

    for (int i = 0; i < 4; i++) {
        renderer->vertices[renderer->vertex_count + i].position.x = corners[i].x;
        renderer->vertices[renderer->vertex_count + i].position.y = corners[i].y;
        renderer->vertices[renderer->vertex_count + i].tex_coord.x = tex_coords[i][0];
        renderer->vertices[renderer->vertex_count + i].tex_coord.y = tex_coords[i][1];
        renderer->vertices[renderer->vertex_count + i].color = (SDL_Color){255, 255, 255, 255};
    }

    renderer->vertex_count += 4;
}

void renderer_flush(Renderer* renderer) {
    if (renderer->vertex_count == 0) return;

    int indices[MAX_BATCH_SIZE * 6];
    for (int i = 0, j = 0; i < renderer->vertex_count; i += 4, j += 6) {
        indices[j + 0] = i + 0;
        indices[j + 1] = i + 1;
        indices[j + 2] = i + 2;
        indices[j + 3] = i + 0;
        indices[j + 4] = i + 2;
        indices[j + 5] = i + 3;
    }

    SDL_RenderGeometry(renderer->sdl_renderer, renderer->current_texture->sdl_texture, 
                       renderer->vertices, renderer->vertex_count, 
                       indices, renderer->vertex_count / 4 * 6);

    renderer->vertex_count = 0;
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
    renderer_begin_batch(renderer);

    for (int i = 0; i < ecs->entity_count; i++) {
        Entity* entity = &ecs->entities[i];
        Component* render_component = NULL;
        ECSResult render_result = cog_get_component(ecs, entity, COMPONENT_RENDER, &render_component);
        
        if (render_result == ECS_OK && render_component) {
            RenderComponent* render_data = (RenderComponent*)render_component->data;
            Vector2 screen_pos = camera_world_to_screen(&engine->camera, entity->transform.position);
            
            if (render_data->sprite) {
                SDL_Rect dest_rect = {
                    (int)screen_pos.x,
                    (int)screen_pos.y,
                    (int)(render_data->sprite->dest_rect.w * entity->transform.scale.x * engine->camera.zoom),
                    (int)(render_data->sprite->dest_rect.h * entity->transform.scale.y * engine->camera.zoom)
                };
                
                renderer_draw_sprite(renderer, 
                                     render_data->sprite->texture, 
                                     &render_data->sprite->src_rect, 
                                     &dest_rect,
                                     entity->transform.rotation - engine->camera.rotation, 
                                     render_data->sprite->center, 
                                     render_data->sprite->flip);
            }
        }
    }

    renderer_end_batch(renderer);
}

void renderer_cleanup(Renderer* renderer) {
    if (renderer->sdl_renderer) {
        SDL_DestroyRenderer(renderer->sdl_renderer);
        renderer->sdl_renderer = NULL;
    }
    cog_log_info("Renderer cleaned up");
}