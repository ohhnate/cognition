#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} Sprite;

void sprite_render(Sprite* sprite, SDL_Renderer* renderer, int x, int y);
void sprite_render_ex(Sprite* sprite, SDL_Renderer* renderer, int x, int y, double scale, double angle);

#endif // SPRITE_H