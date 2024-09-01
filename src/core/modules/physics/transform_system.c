#include "transform_system.h"
#include "modules/physics/transform_components.h"

void transform_system(Entity* entity, Component** components, int count, void* user_data) {
    Transform* transform = (Transform*)components[0]->data;
    
    // Update transform logic here
}