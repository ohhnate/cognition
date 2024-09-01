#include "renderer.h"
#include "cognition.h"
#include <stdio.h>

int renderer_init(Renderer* renderer, SDL_Window* window) {
    renderer->sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer->sdl_renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    return 1;
}

void renderer_clear(Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer->sdl_renderer);
}

void renderer_present(Renderer* renderer) {
    SDL_RenderPresent(renderer->sdl_renderer);
}

void renderer_draw_rect(Renderer* renderer, SDL_Rect* rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer->sdl_renderer, rect);
}

void renderer_draw_line(Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer->sdl_renderer, x1, y1, x2, y2);
}

void renderer_cleanup(Renderer* renderer) {
    SDL_DestroyRenderer(renderer->sdl_renderer);
    renderer->sdl_renderer = NULL;
}