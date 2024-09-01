#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL.h>
#include <stdbool.h>
#include "core/modules/utils/types.h"

typedef struct {
    float x, y;
} Vector2D;

typedef struct {
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    float mass;
    SDL_Rect collider;
    bool is_static;
} PhysicsBody;

typedef struct PhysicsWorld {
    PhysicsBody* bodies[MAX_ENTITIES];
    int body_count;
    Vector2D gravity;
    float time_step;
} PhysicsWorld;

void physics_world_init(PhysicsWorld* world);
int physics_body_create(PhysicsWorld* world, float x, float y, float width, float height, float mass, bool is_static);
void physics_world_update(PhysicsWorld* world);
void apply_force(PhysicsBody* body, Vector2D force);
bool check_collision(PhysicsBody* a, PhysicsBody* b);
void resolve_collision(PhysicsBody* a, PhysicsBody* b);
void physics_world_cleanup(PhysicsWorld* world);

#endif // PHYSICS_H