#include "cog_sprite.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"

Sprite* sprite_create(Texture* texture) {
    Sprite* sprite = COG_NEW(Sprite);
    if (!sprite) {
        cog_log_error("Failed to allocate memory for sprite");
        return NULL;
    }

    sprite->texture = texture;
    sprite->src_rect = (SDL_Rect){0, 0, texture->width, texture->height};
    sprite->dest_rect = (SDL_Rect){0, 0, texture->width, texture->height};
    sprite->angle = 0.0;
    sprite->center = NULL;
    sprite->flip = SDL_FLIP_NONE;

    return sprite;
}

void sprite_destroy(Sprite* sprite) {
    if (sprite) {
        COG_DELETE(sprite);
    }
}

void sprite_render(Sprite* sprite, SDL_Renderer* renderer) {
    SDL_RenderCopyEx(renderer, sprite->texture->sdl_texture, &sprite->src_rect, &sprite->dest_rect, 
                     sprite->angle, sprite->center, sprite->flip);
}

void sprite_set_position(Sprite* sprite, int x, int y) {
    sprite->dest_rect.x = x;
    sprite->dest_rect.y = y;
}

void sprite_set_size(Sprite* sprite, int width, int height) {
    sprite->dest_rect.w = width;
    sprite->dest_rect.h = height;
}

void sprite_set_angle(Sprite* sprite, double angle) {
    sprite->angle = angle;
}

void sprite_set_flip(Sprite* sprite, SDL_RendererFlip flip) {
    sprite->flip = flip;
}

void sprite_set_texture(Sprite* sprite, Texture* texture) {
    sprite->texture = texture;
    sprite->src_rect = (SDL_Rect){0, 0, texture->width, texture->height};
}

void sprite_set_source_rect(Sprite* sprite, SDL_Rect src_rect) {
    sprite->src_rect = src_rect;
}

void sprite_set_center(Sprite* sprite, SDL_Point center) {
    sprite->center = &center;
}