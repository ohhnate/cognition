#include "core/modules/resource/resource_manager.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include <string.h>
#include <stdio.h>

void resource_manager_init(ResourceManager* manager, SDL_Renderer* renderer) {
    manager->image_count = 0;
    manager->renderer = renderer;
    for (int i = 0; i < MAX_RESOURCES; i++) {
        manager->images[i].key = NULL;
        manager->images[i].texture = NULL;
    }
    cog_log_info("Resource manager initialized");
}

SDL_Texture* resource_manager_load_image(ResourceManager* manager, const char* key, const char* file_path) {
    // Check if the image is already loaded
    SDL_Texture* existing = resource_manager_get_image(manager, key);
    if (existing != NULL) {
        return existing;
    }

    // Load the image
    SDL_Surface* loaded_surface = IMG_Load(file_path);
    if (loaded_surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", file_path, IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(manager->renderer, loaded_surface);
    if (texture == NULL) {
        cog_log_error("Unable to create texture from %s! SDL Error: %s", file_path, SDL_GetError());
        SDL_FreeSurface(loaded_surface);
        return NULL;
    }

    // Store the texture in the resource manager
    if (manager->image_count < MAX_RESOURCES) {
        int index = manager->image_count;
        manager->images[index].key = _strdup(key);
        manager->images[index].texture = texture;
        
        // Initialize the sprite
        manager->images[index].sprite.texture = texture;
        manager->images[index].sprite.width = loaded_surface->w;
        manager->images[index].sprite.height = loaded_surface->h;

        manager->image_count++;
        SDL_FreeSurface(loaded_surface);
        return texture;
    } else {
        printf("Resource manager is full. Cannot add more images.\n");
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(loaded_surface);
        return NULL;
    }
}

SDL_Texture* resource_manager_get_image(ResourceManager* manager, const char* key) {
    for (int i = 0; i < manager->image_count; i++) {
        if (strcmp(manager->images[i].key, key) == 0) {
            return manager->images[i].texture;
        }
    }
    return NULL;
}

Sprite* resource_manager_get_sprite(ResourceManager* manager, const char* key) {
    for (int i = 0; i < manager->image_count; i++) {
        if (strcmp(manager->images[i].key, key) == 0) {
            return &manager->images[i].sprite;
        }
    }
    return NULL;
}

void resource_manager_cleanup(ResourceManager* manager) {
    for (int i = 0; i < manager->image_count; i++) {
        free(manager->images[i].key);
        SDL_DestroyTexture(manager->images[i].texture);
    }
    manager->image_count = 0;
}