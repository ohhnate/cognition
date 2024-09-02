#include "core/ecs.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include <string.h>
#include <stdio.h>

ECSResult ecs_init(ECS* ecs) {
    if (ecs == NULL) {
        cog_log_error("ECS pointer is NULL");
        return ECS_ERROR_NULL_POINTER;
    }
    memset(ecs, 0, sizeof(ECS));
    cog_log_info("ECS initialized successfully");
    return ECS_OK;
}

ECSResult create_entity(ECS* ecs, Entity** out_entity) {
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
    ecs->entity_count++;
    *out_entity = entity;
    cog_log_debug("Entity created with ID: %d", entity->id);
    return ECS_OK;
}

ECSResult add_component(ECS* ecs, Entity* entity, Component component) {
    if (ecs == NULL || entity == NULL) return ECS_ERROR_NULL_POINTER;
    if (entity->id >= (uint32_t)ecs->entity_count) return ECS_ERROR_INVALID_ENTITY;
    
    ecs->components[entity->id][component.type] = component;
    return ECS_OK;
}

ECSResult get_component(ECS* ecs, Entity* entity, ComponentType type, Component** out_component) {
    if (ecs == NULL || entity == NULL || out_component == NULL) return ECS_ERROR_NULL_POINTER;
    if ((unsigned int)ecs->entity_count >= MAX_ENTITIES) return ECS_ERROR_INVALID_ENTITY;
    
    *out_component = &ecs->components[entity->id][type];
    return ECS_OK;
}

ECSResult add_system(ECS* ecs, System system) {
    if (ecs == NULL) return ECS_ERROR_NULL_POINTER;
    if (ecs->system_count >= (int)MAX_SYSTEMS) return ECS_ERROR_SYSTEM_FAILURE;
    
    ecs->systems[ecs->system_count++] = system;
    return ECS_OK;
}

ECSResult ecs_update(ECS* ecs, void* user_data) {
    if (ecs == NULL) return ECS_ERROR_NULL_POINTER;
    
    for (int s = 0; s < ecs->system_count; s++) {
        System* system = &ecs->systems[s];
        printf("Updating system %d\n", s);
        
        for (int e = 0; e < ecs->entity_count; e++) {
            Entity* entity = &ecs->entities[e];
            Component* components[MAX_COMPONENTS];
            int component_count = 0;
            
            for (int c = 0; c < system->required_component_count; c++) {
                Component* component;
                ECSResult result = get_component(ecs, entity, system->required_components[c], &component);
                if (result == ECS_OK) {
                    components[component_count++] = component;
                }
            }
            
            if (component_count == system->required_component_count) {
                printf("Running system for entity %d\n", entity->id);
                system->update(entity, components, component_count, user_data);
            }
        }
    }
    
    return ECS_OK;
}

void ecs_cleanup(ECS* ecs) {
    // Free any allocated resources
    // Reset counters
    ecs->entity_count = 0;
    ecs->system_count = 0;
}