#include "transform_components.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include <stdlib.h>

Component create_transform_component(Vector2 position, float rotation, Vector2 scale) {
    Transform* data = COG_NEW(Transform);
    if (data == NULL) {
        cog_log_error("Failed to allocate memory for transform component");
        Component null_component = {COMPONENT_TRANSFORM, NULL};
        return null_component;
    }
    data->position = position;
    data->rotation = rotation;
    data->scale = scale;
    Component component = {
        .type = COMPONENT_TRANSFORM,
        .data = data
    };
    cog_log_debug("Transform component created at position (%f, %f)", position.x, position.y);
    return component;
}

static inline Transform* get_transform(const Component* component) {
    if (component->type != COMPONENT_TRANSFORM || component->data == NULL) {
        return NULL;
    }
    return (Transform*)component->data;
}

Vector2 get_position(const Component* component) {
    Transform* transform = get_transform(component);
    return transform ? transform->position : (Vector2){0, 0};
}

void set_position(Component* component, Vector2 position) {
    Transform* transform = get_transform(component);
    if (transform) {
        transform->position = position;
    }
}

float get_rotation(const Component* component) {
    Transform* transform = get_transform(component);
    return transform ? transform->rotation : 0.0f;
}

void set_rotation(Component* component, float rotation) {
    Transform* transform = get_transform(component);
    if (transform) {
        transform->rotation = rotation;
    }
}

Vector2 get_scale(const Component* component) {
    Transform* transform = get_transform(component);
    return transform ? transform->scale : (Vector2){1.0f, 1.0f};
}

void set_scale(Component* component, Vector2 scale) {
    Transform* transform = get_transform(component);
    if (transform) {
        transform->scale = scale;
    }
}

void transform_cleanup(Component* component) {
    if (component == NULL) {
        cog_log_error("Attempted to clean up NULL component");
        return;
    }

    if (component->type == COMPONENT_TRANSFORM && component->data != NULL) {
        COG_DELETE(component->data);
        component->data = NULL;
        cog_log_debug("Transform component cleaned up successfully");
    } else {
        cog_log_warning("Attempted to clean up invalid transform component");
    }
}