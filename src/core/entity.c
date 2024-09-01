#include "entity.h"
#include <stdlib.h>

void entity_manager_init(EntityManager* manager) {
    manager->entity_count = 0;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        manager->entities[i] = NULL;
    }
}

int entity_manager_create_entity(EntityManager* manager, float x, float y, float width, float height) {
    if (manager->entity_count >= MAX_ENTITIES) {
        return -1;  // No more space for entities
    }

    Entity* new_entity = (Entity*)malloc(sizeof(Entity));
    if (new_entity == NULL) {
        return -1;  // Memory allocation failed
    }

    new_entity->id = manager->entity_count;
    new_entity->x = x;
    new_entity->y = y;
    new_entity->width = width;
    new_entity->height = height;

    manager->entities[manager->entity_count] = new_entity;
    manager->entity_count++;

    return new_entity->id;
}

void entity_manager_update(EntityManager* manager) {
    // Update logic for entities
    for (int i = 0; i < manager->entity_count; i++) {
        Entity* entity = manager->entities[i];
        if (entity != NULL) {
            // Update entity logic here
        }
    }
}

void entity_manager_render(EntityManager* manager, SDL_Renderer* renderer) {
    // Render logic for entities
    for (int i = 0; i < manager->entity_count; i++) {
        Entity* entity = manager->entities[i];
        if (entity != NULL) {
            SDL_Rect rect = {(int)entity->x, (int)entity->y, (int)entity->width, (int)entity->height};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void entity_manager_shutdown(EntityManager* manager) {
    for (int i = 0; i < manager->entity_count; i++) {
        free(manager->entities[i]);
        manager->entities[i] = NULL;
    }
    manager->entity_count = 0;
}