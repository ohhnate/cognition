#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "core/ecs.h"

void physics_system(Entity* entity, Component** components, int count, void* user_data);

#endif // PHYSICS_SYSTEM_H