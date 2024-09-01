#include "sprite.h"

void sprite_render(Sprite* sprite, SDL_Renderer* renderer, int x, int y) {
    SDL_Rect dest_rect = {x, y, sprite->width, sprite->height};
    SDL_RenderCopy(renderer, sprite->texture, NULL, &dest_rect);
}

void sprite_render_ex(Sprite* sprite, SDL_Renderer* renderer, int x, int y, double scale, double angle) {
    SDL_Rect dest_rect = {x, y, (int)(sprite->width * scale), (int)(sprite->height * scale)};
    SDL_RenderCopyEx(renderer, sprite->texture, NULL, &dest_rect, angle, NULL, SDL_FLIP_NONE);
}