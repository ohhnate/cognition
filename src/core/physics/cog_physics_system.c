#include "cog_physics_system.h"
#include "cog_physics_components.h"
#include "core/utils/cog_error.h"
#include <stdio.h>

void physics_system(Entity* entity, Component** components, int count, void* user_data, float delta_time) {
    PhysicsBody* physics = NULL;

    // Search for the physics component
    for (int i = 0; i < count; i++) {
        if (components[i]->type == COMPONENT_PHYSICS) {
            physics = (PhysicsBody*)components[i]->data;
            break;
        }
    }

    if (!physics) {
        // cog_log_debug("No physics component found for entity %d", entity->id);
        return;
    }

    cog_log_debug("Found physics component. Velocity: (%f, %f)", physics->velocity.x, physics->velocity.y);
    
    physics->velocity.x += physics->acceleration.x * delta_time;
    physics->velocity.y += physics->acceleration.y * delta_time;
    
    // Update entity transform based on physics
    entity->transform.position.x += physics->velocity.x * delta_time;
    entity->transform.position.y += physics->velocity.y * delta_time;

    cog_log_debug("Updated position: (%f, %f)", entity->transform.position.x, entity->transform.position.y);
}