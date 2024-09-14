#ifndef SPRITE_H
#define SPRITE_H

#include "cog_texture.h"

typedef struct {
    Texture* texture;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    double angle;
    SDL_Point* center;
    SDL_RendererFlip flip;
} Sprite;

Sprite* sprite_create(Texture* texture);
void sprite_destroy(Sprite* sprite);
void sprite_render(Sprite* sprite, SDL_Renderer* renderer);
void sprite_set_position(Sprite* sprite, int x, int y);
void sprite_set_size(Sprite* sprite, int width, int height);
void sprite_set_angle(Sprite* sprite, double angle);
void sprite_set_flip(Sprite* sprite, SDL_RendererFlip flip);
void sprite_set_texture(Sprite* sprite, Texture* texture);
void sprite_set_source_rect(Sprite* sprite, SDL_Rect src_rect);
void sprite_set_center(Sprite* sprite, SDL_Point center);

#endif // SPRITE_H