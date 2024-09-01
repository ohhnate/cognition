// src/core/modules/graphics/renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "core/ecs.h"

typedef struct CognitionEngine CognitionEngine;

typedef struct Renderer {
    SDL_Renderer* sdl_renderer;
} Renderer;

int renderer_init(Renderer* renderer, SDL_Window* window);
void clear(Renderer* renderer, SDL_Color color);
void present(Renderer* renderer);
void draw_rect(Renderer* renderer, SDL_Rect* rect, SDL_Color color);
void draw_line(Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color);

void render_entities(Renderer* renderer, ECS* ecs, CognitionEngine* engine);

void renderer_cleanup(Renderer* renderer);

#endif // RENDERER_H