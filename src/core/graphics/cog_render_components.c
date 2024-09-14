#include "cog_render_components.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"

Component create_render_component(Sprite* sprite, int layer_id, float z_order) {
    RenderComponent* data = COG_NEW(RenderComponent);
    if (data == NULL) {
        cog_log_error("Failed to allocate memory for render component");
        Component null_component = {COMPONENT_RENDER, NULL};
        return null_component;
    }
    data->sprite = sprite;
    data->layer_id = layer_id;
    data->z_order = z_order;

    Component component = {
        .type = COMPONENT_RENDER,
        .data = data
    };
    cog_log_debug("Render component created");
    return component;
}

void render_component_set_sprite(Component* component, Sprite* sprite) {
    if (component->type == COMPONENT_RENDER && component->data != NULL) {
        RenderComponent* render_data = (RenderComponent*)component->data;
        render_data->sprite = sprite;
    }
}

void render_component_cleanup(Component* component) {
    if (component->type == COMPONENT_RENDER && component->data != NULL) {
        // Note: We don't destroy the sprite here as it might be shared
        COG_DELETE(component->data);
        component->data = NULL;
        cog_log_debug("Render component cleaned up");
    }
}