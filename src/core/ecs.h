// ecs.h
#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include "core/modules/utils/types.h"

struct Sprite;

typedef enum {
    ECS_OK,
    ECS_ERROR_NULL_POINTER,
    ECS_ERROR_INVALID_ENTITY,
    ECS_ERROR_SYSTEM_FAILURE
} ECSResult;

typedef struct {
    uint32_t id;
} Entity;

typedef enum {
    COMPONENT_TRANSFORM,
    COMPONENT_PHYSICS,
    COMPONENT_RENDER,
} ComponentType;

typedef struct {
    ComponentType type;
    void* data;
} Component;

typedef void (*SystemFunction)(Entity*, Component**, int, void*);

typedef struct {
    SystemFunction update;
    ComponentType required_components[MAX_COMPONENTS];
    int required_component_count;
} System;

typedef struct {
    Entity entities[MAX_ENTITIES];
    Component components[MAX_ENTITIES][MAX_COMPONENTS];
    System systems[MAX_SYSTEMS];
    int entity_count;
    int system_count;
} ECS;

// ECS core functions
ECSResult ecs_init(ECS* ecs);
ECSResult create_entity(ECS* ecs, Entity** out_entity);
ECSResult add_component(ECS* ecs, Entity* entity, Component component);
ECSResult get_component(ECS* ecs, Entity* entity, ComponentType type, Component** out_component);
ECSResult add_system(ECS* ecs, System system);
ECSResult ecs_update(ECS* ecs, void* user_data);
void ecs_cleanup(ECS* ecs);

// Component-specific functions
Component create_transform_component(Vector2 position, float rotation, Vector2 scale);
Component create_physics_component(Vector2 velocity, Vector2 acceleration);

// System-specific functions
void transform_system(Entity* entity, Component** components, int count, void* user_data);
void physics_system(Entity* entity, Component** components, int count, void* user_data);
void render_system(Entity* entity, Component** components, int count, void* user_data);

#endif // ECS_H