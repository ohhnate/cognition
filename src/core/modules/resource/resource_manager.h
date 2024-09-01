// resource_manager.h
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include "..\graphics\sprite.h"

#define MAX_RESOURCES 100

typedef struct {
    char* key;
    SDL_Texture* texture;
    Sprite sprite;
} ImageResource;

typedef struct ResourceManager {
    ImageResource images[MAX_RESOURCES];
    int image_count;
    SDL_Renderer* renderer;
} ResourceManager;

void resource_manager_init(ResourceManager* manager, SDL_Renderer* renderer);
SDL_Texture* resource_manager_load_image(ResourceManager* manager, const char* key, const char* file_path);
SDL_Texture* resource_manager_get_image(ResourceManager* manager, const char* key);
Sprite* resource_manager_get_sprite(ResourceManager* manager, const char* key);
void resource_manager_cleanup(ResourceManager* manager);

#endif // RESOURCE_MANAGER_H