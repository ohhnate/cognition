#include "cog_physics_components.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"

Component create_physics_component(Vector2 velocity, Vector2 acceleration, float mass, bool is_static) {
    PhysicsBody* data = COG_NEW(PhysicsBody);
    if (data == NULL) {
        cog_log_error("Failed to allocate memory for physics component");
        Component null_component = {COMPONENT_PHYSICS, NULL};
        return null_component;
    }
    data->velocity = velocity;
    data->acceleration = acceleration;
    data->mass = mass;
    data->is_static = is_static;
    data->position = (Vector2){0, 0}; 
    data->shape = shape_create_circle((Vector2){0, 0}, 1.0f); // Default to a unit circle at (0, 0)
    data->restitution = 0.5f;
   
    Component component = {
        .type = COMPONENT_PHYSICS,
        .data = data
    };
    cog_log_debug("Physics component created");
    return component;
}

void physics_component_set_velocity(Component* component, Vector2 velocity) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        PhysicsBody* body = (PhysicsBody*)component->data;
        body->velocity = velocity;
    }
}

void physics_component_set_acceleration(Component* component, Vector2 acceleration) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        PhysicsBody* body = (PhysicsBody*)component->data;
        body->acceleration = acceleration;
    }
}

void physics_component_set_mass(Component* component, float mass) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        PhysicsBody* body = (PhysicsBody*)component->data;
        body->mass = mass;
    }
}

void physics_component_set_static(Component* component, bool is_static) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        PhysicsBody* body = (PhysicsBody*)component->data;
        body->is_static = is_static;
    }
}

void physics_component_set_shape(Component* component, Shape shape) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        PhysicsBody* body = (PhysicsBody*)component->data;
        body->shape = shape;
    }
}

void physics_component_cleanup(Component* component) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        COG_DELETE(component->data);
        component->data = NULL;
        cog_log_debug("Physics component cleaned up successfully");
    } else {
        cog_log_warning("Attempted to clean up invalid physics component");
    }
}