#ifndef COG_RENDERER_H
#define COG_RENDERER_H

#include <SDL.h>
#include "core/ecs/cog_ecs.h"
#include "core/graphics/cog_texture.h"

#define MAX_BATCH_SIZE 1000

struct CognitionEngine;

typedef struct Renderer {
    SDL_Renderer* sdl_renderer;
    SDL_Vertex vertices[MAX_BATCH_SIZE * 4];
    int vertex_count;
    Texture* current_texture;
} Renderer;

int renderer_init(Renderer* renderer, SDL_Window* window);
void renderer_begin_batch(Renderer* renderer);
void renderer_end_batch(Renderer* renderer);
void renderer_draw_sprite(Renderer* renderer, Texture* texture, SDL_Rect* src_rect, SDL_Rect* dest_rect, double angle, SDL_Point* center, SDL_RendererFlip flip);
void renderer_flush(Renderer* renderer);
void clear(Renderer* renderer, SDL_Color color);
void present(Renderer* renderer);
void draw_rect(Renderer* renderer, SDL_Rect* rect, SDL_Color color);
void draw_line(Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void render_entities(Renderer* renderer, ECS* ecs, struct CognitionEngine* engine);
void renderer_cleanup(Renderer* renderer);

#endif // COG_RENDERER_H