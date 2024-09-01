#include "render_system.h"
#include "core/cognition.h"
#include "modules/graphics/renderer.h"
#include "modules/resource/resource_manager.h"
#include "modules/graphics/render_components.h"
#include "modules/physics/transform_components.h"

void render_system(Entity* entity, Component** components, int count, void* user_data) {
    CognitionEngine* engine = (CognitionEngine*)user_data;
    Transform* transform = (Transform*)components[0]->data;
    RenderComponentData* render = (RenderComponentData*)components[1]->data;
    
    Sprite* sprite = resource_manager_get_sprite(engine->resource_manager, render->sprite_id);
    
    if (sprite) {
        sprite_render(sprite, engine->renderer->sdl_renderer, 
                      (int)transform->position.x, 
                      (int)transform->position.y);
    }
}