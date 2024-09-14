#include "cog_render_system.h"
#include "core/cognition.h"
#include "core/graphics/cog_renderer.h"
#include "core/resource/cog_resource.h"
#include "cog_render_components.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include <string.h>

void render_system(Entity* entity, Component** components, int count, void* user_data, float delta_time) {
    CognitionEngine* engine = (CognitionEngine*)user_data;
    RenderComponent* render = (RenderComponent*)components[0]->data;
   
    if (render->sprite) {
        // Update sprite position based on entity transform
        sprite_set_position(render->sprite, (int)entity->transform.position.x, (int)entity->transform.position.y);
        sprite_set_angle(render->sprite, entity->transform.rotation);
       
        // Apply scale
        int width = (int)(render->sprite->dest_rect.w * entity->transform.scale.x);
        int height = (int)(render->sprite->dest_rect.h * entity->transform.scale.y);
        sprite_set_size(render->sprite, width, height);
    }
}

RenderLayerManager* render_layer_manager_create() {
    RenderLayerManager* manager = (RenderLayerManager*)cog_malloc(sizeof(RenderLayerManager));
    if (!manager) {
        cog_log_error("Failed to allocate memory for RenderLayerManager");
        return NULL;
    }
    
    manager->layers = (RenderLayer*)cog_malloc(sizeof(RenderLayer) * 10);  // Start with 10 layers
    if (!manager->layers) {
        cog_log_error("Failed to allocate memory for RenderLayers");
        cog_free(manager);
        return NULL;
    }
    
    manager->count = 0;
    manager->capacity = 10;
    return manager;
}

void render_layer_manager_destroy(RenderLayerManager* manager) {
    if (manager) {
        for (int i = 0; i < manager->count; i++) {
            cog_free(manager->layers[i].name);
        }
        cog_free(manager->layers);
        cog_free(manager);
    }
}

int render_layer_add(RenderLayerManager* manager, const char* name, float order) {
    if (manager->count >= manager->capacity) {
        int new_capacity = manager->capacity * 2;
        RenderLayer* new_layers = (RenderLayer*)cog_realloc(manager->layers, sizeof(RenderLayer) * new_capacity);
        if (!new_layers) {
            cog_log_error("Failed to resize RenderLayers array");
            return -1;
        }
        manager->layers = new_layers;
        manager->capacity = new_capacity;
    }

    RenderLayer* layer = &manager->layers[manager->count];
    layer->id = manager->count;
    layer->name = cog_strdup(name);
    layer->order = order;
    layer->is_visible = true;

    return manager->count++;
}

void render_layer_remove(RenderLayerManager* manager, int layer_id) {
    if (layer_id < 0 || layer_id >= manager->count) return;

    cog_free(manager->layers[layer_id].name);

    // Shift remaining layers
    for (int i = layer_id; i < manager->count - 1; i++) {
        manager->layers[i] = manager->layers[i + 1];
        manager->layers[i].id = i;
    }

    manager->count--;
}

void render_layer_set_visible(RenderLayerManager* manager, int layer_id, bool visible) {
    if (layer_id >= 0 && layer_id < manager->count) {
        manager->layers[layer_id].is_visible = visible;
    }
}

bool render_layer_is_visible(RenderLayerManager* manager, int layer_id) {
    if (layer_id >= 0 && layer_id < manager->count) {
        return manager->layers[layer_id].is_visible;
    }
    return false;
}

// Helper function to compare RenderComponents for sorting
static int compare_render_components(const void* a, const void* b) {
    RenderComponent* comp_a = *(RenderComponent**)a;
    RenderComponent* comp_b = *(RenderComponent**)b;

    if (comp_a->layer_id != comp_b->layer_id) {
        return comp_a->layer_id - comp_b->layer_id;
    }

    if (comp_a->z_order < comp_b->z_order) return -1;
    if (comp_a->z_order > comp_b->z_order) return 1;
    return 0;
}

void render_system_update(ECS* ecs, RenderLayerManager* layer_manager, Renderer* renderer) {
    // Collect all render components
    RenderComponent* render_components[MAX_ENTITIES];
    int render_component_count = 0;

    for (int i = 0; i < ecs->entity_count; i++) {
        Entity* entity = &ecs->entities[i];
        Component* render_component = NULL;
        ECSResult render_result = cog_get_component(ecs, entity, COMPONENT_RENDER, &render_component);
        
        if (render_result == ECS_OK && render_component) {
            render_components[render_component_count++] = (RenderComponent*)render_component->data;
        }
    }

    // Sort render components
    qsort(render_components, render_component_count, sizeof(RenderComponent*), compare_render_components);

    // Render sorted components
    for (int i = 0; i < render_component_count; i++) {
        RenderComponent* component = render_components[i];
        if (render_layer_is_visible(layer_manager, component->layer_id)) {
            sprite_render(component->sprite, renderer->sdl_renderer);
        }
    }
}