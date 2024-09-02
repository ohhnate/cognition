#include "render_components.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include <stdlib.h>
#include <string.h>

Component create_render_component(const char* sprite_id) {
    RenderComponentData* data = COG_NEW(RenderComponentData);
    if (data == NULL) {
        cog_log_error("Failed to allocate memory for render component");
        Component null_component = {COMPONENT_RENDER, NULL};
        return null_component;
    }

    data->sprite_id = cog_strdup(sprite_id);
    if (data->sprite_id == NULL) {
        cog_log_error("Failed to allocate memory for sprite ID");
        COG_DELETE(data);
        Component null_component = {COMPONENT_RENDER, NULL};
        return null_component;
    }

    Component component = {
        .type = COMPONENT_RENDER,
        .data = data
    };

    cog_log_debug("Render component created with sprite ID: %s", sprite_id);
    return component;
}

const char* render_component_get_sprite_id(const Component* component) {
    if (component->type != COMPONENT_RENDER || component->data == NULL) {
        cog_log_error("Invalid render component or NULL data");
        return NULL; // Invalid component
    }
    RenderComponentData* data = (RenderComponentData*)component->data;
    return data->sprite_id;
}

void render_component_set_sprite_id(Component* component, const char* new_sprite_id) {
    if (component->type != COMPONENT_RENDER || component->data == NULL) {
        cog_log_error("Invalid render component or NULL data");
        return; // Invalid component
    }
    RenderComponentData* data = (RenderComponentData*)component->data;
    if (data->sprite_id) {
        COG_DELETE((void*)data->sprite_id);  // Free the old string
    }
    data->sprite_id = cog_strdup(new_sprite_id);  // Make a copy of the new string
    if (data->sprite_id == NULL) {
        cog_log_error("Failed to allocate memory for new sprite ID");
    } else {
        cog_log_debug("Sprite ID updated to: %s", data->sprite_id);
    }
}

void render_component_cleanup(Component* component) {
    if (component->type == COMPONENT_RENDER && component->data != NULL) {
        RenderComponentData* data = (RenderComponentData*)component->data;
        if (data->sprite_id) {
            COG_DELETE((void*)data->sprite_id);
        }
        COG_DELETE(data);
        component->data = NULL;
        cog_log_debug("Render component cleaned up");
    } else {
        cog_log_warning("Attempted to clean up invalid render component");
    }
}