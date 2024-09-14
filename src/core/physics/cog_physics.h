#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL.h>
#include <stdbool.h>
#include "core/utils/cog_types.h"
#include "cog_shapes.h"

typedef struct {
    Shape shape;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    float restitution;
    bool is_static;
} PhysicsBody;

typedef struct PhysicsWorld {
    PhysicsBody* bodies[MAX_ENTITIES];
    int body_count;
    Vector2 gravity;
    float time_step;
} PhysicsWorld;

void physics_world_init(PhysicsWorld* world);
int physics_body_create(PhysicsWorld* world, Shape shape, Vector2 position, float mass, bool is_static);
void physics_world_update(PhysicsWorld* world, double delta_time);
void apply_force(PhysicsBody* body, Vector2 force);
bool check_collision(PhysicsBody* a, PhysicsBody* b);
void resolve_collision(PhysicsBody* a, PhysicsBody* b);
void physics_world_cleanup(PhysicsWorld* world);

#endif // PHYSICS_H