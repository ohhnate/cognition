#ifndef RENDER_COMPONENTS_H
#define RENDER_COMPONENTS_H

#include "core/ecs.h"

typedef struct {
    const char* sprite_id;  // Identifier for the sprite in the resource manager
} RenderComponentData;

Component create_render_component(const char* sprite_id);
const char* render_component_get_sprite_id(const Component* component);
void render_component_set_sprite_id(Component* component, const char* new_sprite_id);
void render_component_cleanup(Component* component);

#endif // RENDER_COMPONENTS_H