#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

// Define the Renderer struct here
typedef struct Renderer {
    SDL_Renderer* sdl_renderer;
} Renderer;

// Function declarations
int renderer_init(Renderer* renderer, SDL_Window* window);
void renderer_clear(Renderer* renderer, SDL_Color color);
void renderer_present(Renderer* renderer);
void renderer_draw_rect(Renderer* renderer, SDL_Rect* rect, SDL_Color color);
void renderer_draw_line(Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void renderer_cleanup(Renderer* renderer);

#endif // RENDERER_H