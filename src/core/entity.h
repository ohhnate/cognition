#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#define MAX_ENTITIES 1000

typedef struct {
    int id;
    float x, y;
    float width, height;
} Entity;

typedef struct EntityManager {
    Entity* entities[MAX_ENTITIES];
    int entity_count;
} EntityManager;

// Initialize the entity manager
void entity_manager_init(EntityManager* manager);

// Create a new entity and return its ID
int entity_manager_create_entity(EntityManager* manager, float x, float y, float width, float height);

// Update all entities
void entity_manager_update(EntityManager* manager);

// Render all entities
void entity_manager_render(EntityManager* manager, SDL_Renderer* renderer);

// Clean up the entity manager
void entity_manager_shutdown(EntityManager* manager);

#endif // ENTITY_H