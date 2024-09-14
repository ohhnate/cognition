#include "cog_ecs.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include "core/utils/cog_math.h"
#include <string.h>
#include <math.h>

ECSResult cog_ecs_init(ECS* ecs) {
    if (ecs == NULL) {
        cog_log_error("ECS pointer is NULL");
        return ECS_ERROR_NULL_POINTER;
    }
    memset(ecs, 0, sizeof(ECS));
    cog_log_info("ECS initialized successfully");
    return ECS_OK;
}

ECSResult cog_create_entity(ECS* ecs, Entity** out_entity) {
    if (ecs == NULL || out_entity == NULL) {
        cog_log_error("Invalid parameters in create_entity");
        return ECS_ERROR_NULL_POINTER;
    }
    if (ecs->entity_count >= MAX_ENTITIES) {
        cog_log_error("Maximum entity count reached");
        return ECS_ERROR_INVALID_ENTITY;
    }
    
    Entity* entity = &ecs->entities[ecs->entity_count];
    entity->id = ecs->entity_count;
    entity->transform = (Transform){{0, 0}, 0, {1, 1}}; 
    ecs->entity_count++;
    *out_entity = entity;
    cog_log_debug("Entity created with ID: %d", entity->id);
    return ECS_OK;
}

ECSResult cog_add_component(ECS* ecs, Entity* entity, Component component) {
    if (ecs == NULL || entity == NULL) return ECS_ERROR_NULL_POINTER;
    if (entity->id >= ecs->entity_count) return ECS_ERROR_INVALID_ENTITY;
    
    ecs->components[entity->id][component.type] = component;
    return ECS_OK;
}

ECSResult cog_get_component(ECS* ecs, Entity* entity, ComponentType type, Component** out_component) {
    if (ecs == NULL || entity == NULL || out_component == NULL) return ECS_ERROR_NULL_POINTER;
    if (entity->id >= ecs->entity_count) return ECS_ERROR_INVALID_ENTITY;
    
    *out_component = &ecs->components[entity->id][type];
    return ECS_OK;
}

ECSResult cog_add_system(ECS* ecs, System system) {
    if (ecs == NULL) return ECS_ERROR_NULL_POINTER;
    if (ecs->system_count >= MAX_SYSTEMS) return ECS_ERROR_SYSTEM_FAILURE;
    
    ecs->systems[ecs->system_count++] = system;
    return ECS_OK;
}

void cog_ecs_update(ECS* ecs, void* user_data, float delta_time) {
    for (int e = 0; e < ecs->entity_count; e++) {
        Entity* entity = &ecs->entities[e];
        for (int s = 0; s < ecs->system_count; s++) {
            System* system = &ecs->systems[s];
            
            // Collect all components for this entity
            Component* entity_components[MAX_COMPONENTS];
            int component_count = 0;
            for (int c = 0; c < COMPONENT_COUNT; c++) {
                if (ecs->components[entity->id][c].data != NULL) {
                    entity_components[component_count++] = &ecs->components[entity->id][c];
                }
            }
            system->update(entity, entity_components, component_count, user_data, delta_time);
        }
    }
}

void cog_ecs_cleanup(ECS* ecs) {
    ecs->entity_count = 0;
    ecs->system_count = 0;
}


// Transform functions
void transform_set_position(Transform* transform, Vector2 position) {
    transform->position = position;
}

void transform_set_rotation(Transform* transform, float rotation) {
    transform->rotation = rotation;
}

void transform_set_scale(Transform* transform, Vector2 scale) {
    transform->scale = scale;
}

Vector2 transform_get_forward(Transform* transform) {
    float angle = transform->rotation * DEG2RAD;
    return (Vector2){cosf(angle), sinf(angle)};
}

Vector2 transform_get_right(Transform* transform) {
    float angle = (transform->rotation + 90) * DEG2RAD;
    return (Vector2){cosf(angle), sinf(angle)};
}