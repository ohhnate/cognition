#ifndef COG_RESOURCE_H
#define COG_RESOURCE_H

#include <SDL.h>
#include "core/graphics/cog_texture.h"

#define MAX_RESOURCES 256

typedef struct {
    char* key;
    Texture* texture;
} TextureResource;

typedef struct ResourceManager {
    TextureResource textures[MAX_RESOURCES];
    int texture_count;
    SDL_Renderer* renderer;
} ResourceManager;

// Function declarations (not definitions)
ResourceManager* resource_manager_create(SDL_Renderer* renderer);
void resource_manager_destroy(ResourceManager* manager);
Texture* resource_manager_load_texture(ResourceManager* manager, const char* key, const char* file_path);
Texture* resource_manager_get_texture(ResourceManager* manager, const char* key);
void resource_manager_cleanup(ResourceManager* manager);

#endif // COG_RESOURCE_H