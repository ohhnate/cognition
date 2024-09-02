#include "transform_system.h"
#include "modules/physics/transform_components.h"
#include "core/modules/utils/error_handling.h"

void transform_system(Entity* entity, Component** components, int count, void* user_data) {
    if (count < 1) {
        cog_log_error("Not enough components for transform system. Expected at least 1, got %d", count);
        return;
    }

    Transform* transform = (Transform*)components[0]->data;
    
    // Update transform logic here
    cog_log_debug("Transform system updated for entity %d. Position: (%f, %f)", 
                  entity->id, transform->position.x, transform->position.y);
}