#include "physics_components.h"
#include <stdlib.h>

Component create_physics_component(Vector2D position, Vector2D velocity, Vector2D acceleration, float mass, bool is_static) {
    PhysicsBody* data = malloc(sizeof(PhysicsBody));
    if (data == NULL) {
        Component null_component = {COMPONENT_PHYSICS, NULL};
        return null_component;
    }
    data->position = position;
    data->velocity = velocity;
    data->acceleration = acceleration;
    data->mass = mass;
    data->is_static = is_static;

    data->collider = (SDL_Rect){(int)position.x, (int)position.y, 10, 10};  
    
    Component component = {
        .type = COMPONENT_PHYSICS,
        .data = data
    };
    return component;
}

void physics_component_set_velocity(Component* component, Vector2D velocity) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        PhysicsBody* body = (PhysicsBody*)component->data;
        body->velocity = velocity;
    }
}

void physics_component_set_acceleration(Component* component, Vector2D acceleration) {
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

void physics_component_cleanup(Component* component) {
    if (component->type == COMPONENT_PHYSICS && component->data != NULL) {
        free(component->data);
        component->data = NULL;
    }
}