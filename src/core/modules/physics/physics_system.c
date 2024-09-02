#include "physics_system.h"
#include "modules/physics/transform_components.h"
#include "modules/physics/physics_components.h"
#include "core/modules/utils/error_handling.h"
#include <stdio.h>

void physics_system(Entity* entity, Component** components, int count, void* user_data) {
    cog_log_debug("Entering physics_system for entity %d", entity->id);

    if (count < 2) {
        cog_log_error("Not enough components for physics system. Expected 2, got %d", count);
        return;
    }

    Transform* transform = NULL;
    PhysicsBody* physics = NULL;

    if (components[0]->type == COMPONENT_TRANSFORM) {
        transform = (Transform*)components[0]->data;
        cog_log_debug("Found transform component. Position: (%f, %f)", transform->position.x, transform->position.y);
    } else {
        cog_log_error("First component is not a transform component");
        return;
    }

    if (components[1]->type == COMPONENT_PHYSICS) {
        physics = (PhysicsBody*)components[1]->data;
        cog_log_debug("Found physics component. Velocity: (%f, %f)", physics->velocity.x, physics->velocity.y);
    } else {
        cog_log_error("Second component is not a physics component");
        return;
    }
    
    cog_log_debug("Updating physics...");
    physics->velocity.x += physics->acceleration.x;
    physics->velocity.y += physics->acceleration.y;
    
    transform->position.x += physics->velocity.x;
    transform->position.y += physics->velocity.y;

    cog_log_debug("Updated position: (%f, %f)", transform->position.x, transform->position.y);
}