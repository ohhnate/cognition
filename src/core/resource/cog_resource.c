
#include "cog_resource.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include <string.h>

ResourceManager* resource_manager_create(SDL_Renderer* renderer) {
    ResourceManager* manager = COG_NEW(ResourceManager);
    if (manager) {
        manager->texture_count = 0;
        manager->renderer = renderer;
        for (int i = 0; i < MAX_RESOURCES; i++) {
            manager->textures[i].key = NULL;
            manager->textures[i].texture = NULL;
        }
    }
    return manager;
}

void resource_manager_destroy(ResourceManager* manager) {
    if (manager) {
        resource_manager_cleanup(manager);
        COG_DELETE(manager);
    }
}

Texture* resource_manager_load_texture(ResourceManager* manager, const char* key, const char* file_path) {
    if (!manager || !key || !file_path) {
        cog_log_error("Invalid parameters in resource_manager_load_texture");
        return NULL;
    }

    // Check if the texture is already loaded
    Texture* existing = resource_manager_get_texture(manager, key);
    if (existing != NULL) {
        return existing;
    }

    if (manager->texture_count >= MAX_RESOURCES) {
        cog_log_error("Maximum texture count reached");
        return NULL;
    }

    Texture* texture = texture_create(manager->renderer, file_path, key);
    if (!texture) {
        return NULL;
    }

    int index = manager->texture_count;
    manager->textures[index].key = cog_strdup(key);
    manager->textures[index].texture = texture;
    manager->texture_count++;

    cog_log_info("Texture loaded: %s", key);
    return texture;
}

Texture* resource_manager_get_texture(ResourceManager* manager, const char* key) {
    if (!manager || !key) return NULL;

    for (int i = 0; i < manager->texture_count; i++) {
        if (strcmp(manager->textures[i].key, key) == 0) {
            return manager->textures[i].texture;
        }
    }
    return NULL;
}

void resource_manager_cleanup(ResourceManager* manager) {
    if (!manager) return;

    for (int i = 0; i < manager->texture_count; i++) {
        cog_free(manager->textures[i].key);
        texture_destroy(manager->textures[i].texture);
        manager->textures[i].key = NULL;
        manager->textures[i].texture = NULL;
    }
    manager->texture_count = 0;
    cog_log_info("Resource manager cleaned up");
}
