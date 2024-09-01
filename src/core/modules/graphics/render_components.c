#include "render_components.h"
#include <stdlib.h>
#include <string.h>

Component create_render_component(const char* sprite_id) {
    RenderComponentData* data = malloc(sizeof(RenderComponentData));
    if (data == NULL) {
        // Handle memory allocation failure
        Component null_component = {COMPONENT_RENDER, NULL};
        return null_component;
    }

    data->sprite_id = _strdup(sprite_id);
    if (data->sprite_id == NULL) {
        // Handle memory allocation failure
        free(data);
        Component null_component = {COMPONENT_RENDER, NULL};
        return null_component;
    }

    Component component = {
        .type = COMPONENT_RENDER,
        .data = data
    };

    return component;
}

const char* render_component_get_sprite_id(const Component* component) {
    if (component->type != COMPONENT_RENDER || component->data == NULL) {
        return NULL; // Invalid component
    }
    RenderComponentData* data = (RenderComponentData*)component->data;
    return data->sprite_id;
}

void render_component_set_sprite_id(Component* component, const char* new_sprite_id) {
    if (component->type != COMPONENT_RENDER || component->data == NULL) {
        return; // Invalid component
    }
    RenderComponentData* data = (RenderComponentData*)component->data;
    free((void*)data->sprite_id);  // Free the old string
    data->sprite_id = _strdup(new_sprite_id);  // Make a copy of the new string
}

void render_component_cleanup(Component* component) {
    if (component->type == COMPONENT_RENDER && component->data != NULL) {
        RenderComponentData* data = (RenderComponentData*)component->data;
        free((void*)data->sprite_id);
        free(data);
        component->data = NULL;
    }
}