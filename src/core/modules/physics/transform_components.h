// transform_components.h
#ifndef TRANSFORM_COMPONENTS_H
#define TRANSFORM_COMPONENTS_H

#include "core/ecs.h"
#include "core/modules/utils/types.h"

typedef struct {
    Vector2 position;
    float rotation;
    Vector2 scale;
} Transform;

Component create_transform_component(Vector2 position, float rotation, Vector2 scale);
Vector2 get_position(const Component* component);
void set_position(Component* component, Vector2 position);
float get_rotation(const Component* component);
void set_rotation(Component* component, float rotation);
Vector2 get_scale(const Component* component);
void set_scale(Component* component, Vector2 scale);
void transform_cleanup(Component* component);

#endif // TRANSFORM_COMPONENTS_H