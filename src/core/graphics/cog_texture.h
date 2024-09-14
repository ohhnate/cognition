#ifndef COG_TEXTURE_H
#define COG_TEXTURE_H

#include <SDL.h>
#include "core/utils/cog_types.h"

typedef struct Texture {
    SDL_Texture* sdl_texture;
    int width;
    int height;
    const char* id;
} Texture;

Texture* texture_create(SDL_Renderer* renderer, const char* path, const char* id);
void texture_destroy(Texture* texture);
void texture_render(Texture* texture, SDL_Renderer* renderer, int x, int y);
void texture_render_ex(Texture* texture, SDL_Renderer* renderer, int x, int y, double angle, SDL_RendererFlip flip);
void texture_set_color(Texture* texture, Color color);
void texture_set_blend_mode(Texture* texture, SDL_BlendMode blendMode);
void texture_set_alpha(Texture* texture, Uint8 alpha);

#endif // COG_TEXTURE_H