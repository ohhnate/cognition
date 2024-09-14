#include "cog_texture.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include <SDL_image.h>

Texture* texture_create(SDL_Renderer* renderer, const char* path, const char* id) {
    Texture* texture = COG_NEW(Texture);
    if (!texture) {
        cog_log_error("Failed to allocate memory for texture");
        return NULL;
    }

    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        cog_log_error("Failed to load image: %s", IMG_GetError());
        COG_DELETE(texture);
        return NULL;
    }

    texture->sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture->sdl_texture) {
        cog_log_error("Failed to create texture: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        COG_DELETE(texture);
        return NULL;
    }

    texture->width = surface->w;
    texture->height = surface->h;
    texture->id = id;

    SDL_FreeSurface(surface);
    return texture;
}

void texture_destroy(Texture* texture) {
    if (texture) {
        SDL_DestroyTexture(texture->sdl_texture);
        COG_DELETE(texture);
    }
}

void texture_render(Texture* texture, SDL_Renderer* renderer, int x, int y) {
    SDL_Rect dest = {x, y, texture->width, texture->height};
    SDL_RenderCopy(renderer, texture->sdl_texture, NULL, &dest);
}

void texture_render_ex(Texture* texture, SDL_Renderer* renderer, int x, int y, double angle, SDL_RendererFlip flip) {
    SDL_Rect dest = {x, y, texture->width, texture->height};
    SDL_RenderCopyEx(renderer, texture->sdl_texture, NULL, &dest, angle, NULL, flip);
}

void texture_set_color(Texture* texture, Color color) {
    SDL_SetTextureColorMod(texture->sdl_texture, color.r, color.g, color.b);
}

void texture_set_blend_mode(Texture* texture, SDL_BlendMode blendMode) {
    SDL_SetTextureBlendMode(texture->sdl_texture, blendMode);
}

void texture_set_alpha(Texture* texture, Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture->sdl_texture, alpha);
}