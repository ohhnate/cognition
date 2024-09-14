#ifndef RENDER_COMPONENTS_H
#define RENDER_COMPONENTS_H

#include "core/ecs/cog_ecs.h"
#include "cog_sprite.h"

typedef struct {
    Sprite* sprite;
    int layer_id;
    float z_order;
} RenderComponent;

Component create_render_component(Sprite* sprite, int layer_id, float z_order);
void render_component_set_sprite(Component* component, Sprite* sprite);
void render_component_cleanup(Component* component);

#endif // RENDER_COMPONENTS_H