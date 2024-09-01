#include "physics_system.h"
#include "modules/physics/transform_components.h"
#include "modules/physics/physics_components.h"
#include <stdio.h>

void physics_system(Entity* entity, Component** components, int count, void* user_data) {
    printf("Entering physics_system for entity %d\n", entity->id);

    if (count < 2) {
        printf("Error: Not enough components for physics system. Expected 2, got %d\n", count);
        return;
    }

    Transform* transform = NULL;
    PhysicsBody* physics = NULL;

    // Safely get the transform component
    if (components[0]->type == COMPONENT_TRANSFORM) {
        transform = (Transform*)components[0]->data;
        printf("Found transform component. Position: (%f, %f)\n", transform->position.x, transform->position.y);
    } else {
        printf("Error: First component is not a transform component\n");
        return;
    }

    // Safely get the physics component
    if (components[1]->type == COMPONENT_PHYSICS) {
        physics = (PhysicsBody*)components[1]->data;
        printf("Found physics component. Velocity: (%f, %f)\n", physics->velocity.x, physics->velocity.y);
    } else {
        printf("Error: Second component is not a physics component\n");
        return;
    }
    
    // Update physics logic here
    printf("Updating physics...\n");
    physics->velocity.x += physics->acceleration.x;
    physics->velocity.y += physics->acceleration.y;
    
    transform->position.x += physics->velocity.x;
    transform->position.y += physics->velocity.y;

    printf("Updated position: (%f, %f)\n", transform->position.x, transform->position.y);
    printf("Exiting physics_system for entity %d\n", entity->id);
}