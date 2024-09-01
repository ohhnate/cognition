#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H

#include "core/ecs.h"
#include "modules/physics/physics.h"

Component create_physics_component(Vector2D position, Vector2D velocity, Vector2D acceleration, float mass, bool is_static);
void physics_component_set_velocity(Component* component, Vector2D velocity);
void physics_component_set_acceleration(Component* component, Vector2D acceleration);
void physics_component_set_mass(Component* component, float mass);
void physics_component_set_static(Component* component, bool is_static);
void physics_component_cleanup(Component* component);

#endif // PHYSICS_COMPONENTS_H