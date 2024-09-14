#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H

#include "core/ecs/cog_ecs.h"
#include "cog_physics.h"

Component create_physics_component(Vector2 velocity, Vector2 acceleration, float mass, bool is_static);
void physics_component_set_velocity(Component* component, Vector2 velocity);
void physics_component_set_acceleration(Component* component, Vector2 acceleration);
void physics_component_set_mass(Component* component, float mass);
void physics_component_set_static(Component* component, bool is_static);
void physics_component_set_shape(Component* component, Shape shape);
void physics_component_cleanup(Component* component);

#endif // PHYSICS_COMPONENTS_H