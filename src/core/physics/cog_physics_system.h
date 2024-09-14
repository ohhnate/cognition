#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "core/ecs/cog_ecs.h"

void physics_system(Entity* entity, Component** components, int count, void* user_data, float delta_time);

#endif // PHYSICS_SYSTEM_H