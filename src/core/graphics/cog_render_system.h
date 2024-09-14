#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "core/ecs/cog_ecs.h"
#include <stdbool.h>

struct Renderer;
struct ECS;

typedef struct {
    int id;
    char* name;
    float order;
    bool is_visible;
} RenderLayer;

typedef struct {
    RenderLayer* layers;
    int count;
    int capacity;
} RenderLayerManager;

void render_system(Entity* entity, Component** components, int count, void* user_data, float delta_time);
RenderLayerManager* render_layer_manager_create();
void render_layer_manager_destroy(RenderLayerManager* manager);
int render_layer_add(RenderLayerManager* manager, const char* name, float order);
void render_layer_remove(RenderLayerManager* manager, int layer_id);
void render_layer_set_visible(RenderLayerManager* manager, int layer_id, bool visible);
bool render_layer_is_visible(RenderLayerManager* manager, int layer_id);
void render_system_update(struct ECS* ecs, RenderLayerManager* layer_manager, struct Renderer* renderer);

#endif // RENDER_SYSTEM_H