#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "core/ecs.h"

void render_system(Entity* entity, Component** components, int count, void* user_data);

#endif // RENDER_SYSTEM_H