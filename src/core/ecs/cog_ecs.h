#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include "core/utils/cog_types.h"

typedef enum {
    ECS_OK,
    ECS_ERROR_NULL_POINTER,
    ECS_ERROR_INVALID_ENTITY,
    ECS_ERROR_SYSTEM_FAILURE
} ECSResult;

typedef struct {
    Vector2 position;
    float rotation;
    Vector2 scale;
} Transform;

typedef struct {
    uint32_t id;
    Transform transform;
} Entity;

typedef enum {
    COMPONENT_RENDER,
    COMPONENT_PHYSICS,
    COMPONENT_COUNT
} ComponentType;

typedef struct {
    ComponentType type;
    void* data;
} Component;

typedef void (*SystemFunction)(Entity*, Component**, int, void*, float);

typedef struct {
    SystemFunction update;
    ComponentType required_components[MAX_COMPONENTS];
    int required_component_count;
} System;

typedef struct {
    Entity entities[MAX_ENTITIES];
    Component components[MAX_ENTITIES][COMPONENT_COUNT];
    System systems[MAX_SYSTEMS];
    unsigned int entity_count;
    unsigned int system_count;
} ECS;

// ECS core functions
ECSResult cog_ecs_init(ECS* ecs);
ECSResult cog_create_entity(ECS* ecs, Entity** out_entity);
ECSResult cog_add_component(ECS* ecs, Entity* entity, Component component);
ECSResult cog_get_component(ECS* ecs, Entity* entity, ComponentType type, Component** out_component);
ECSResult cog_add_system(ECS* ecs, System system);
void cog_ecs_update(ECS* ecs, void* user_data, float delta_time);
void cog_ecs_cleanup(ECS* ecs);

// Transform functions
void transform_set_position(Transform* transform, Vector2 position);
void transform_set_rotation(Transform* transform, float rotation);
void transform_set_scale(Transform* transform, Vector2 scale);
Vector2 transform_get_forward(Transform* transform);
Vector2 transform_get_right(Transform* transform);

#endif // ECS_H