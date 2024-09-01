#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "core/ecs.h"

void transform_system(Entity* entity, Component** components, int count, void* user_data);

#endif // TRANSFORM_SYSTEM_H