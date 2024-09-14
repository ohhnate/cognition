#include "cog_physics.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include "core/utils/cog_math.h"
#include <math.h>

void physics_world_init(PhysicsWorld* world) {
    world->body_count = 0;
    world->gravity = (Vector2){0, 9.8f};
    world->time_step = 1.0f / 60.0f;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        world->bodies[i] = NULL;
    }
    cog_log_info("Physics world initialized");
}

int physics_body_create(PhysicsWorld* world, Shape shape, Vector2 position, float mass, bool is_static) {
    if (world->body_count >= MAX_ENTITIES) {
        cog_log_error("Maximum number of physics bodies reached");
        return -1;
    }

    PhysicsBody* body = COG_NEW(PhysicsBody);
    if (body == NULL) {
        cog_log_error("Failed to allocate memory for physics body");
        return -1;
    }

    body->shape = shape;
    body->position = position;
    body->velocity = (Vector2){0, 0};
    body->acceleration = (Vector2){0, 0};
    body->mass = mass;
    body->restitution = 0.5f;
    body->is_static = is_static;

    world->bodies[world->body_count] = body;
    cog_log_debug("Physics body created at position (%f, %f)", position.x, position.y);
    return world->body_count++;
}

void physics_world_update(PhysicsWorld* world, double delta_time) {
    for (int i = 0; i < world->body_count; i++) {
        PhysicsBody* body = world->bodies[i];
        if (!body->is_static) {

            body->acceleration = vector2_add(body->acceleration, world->gravity);
            body->velocity = vector2_add(body->velocity, vector2_multiply(body->acceleration, world->time_step));
            body->position = vector2_add(body->position, vector2_multiply(body->velocity, world->time_step));
            shape_translate(&body->shape, vector2_multiply(body->velocity, world->time_step));
            body->acceleration = (Vector2){0, 0};
        }
    }

    // Check and resolve collisions
    for (int i = 0; i < world->body_count; i++) {
        for (int j = i + 1; j < world->body_count; j++) {
            if (check_collision(world->bodies[i], world->bodies[j])) {
                resolve_collision(world->bodies[i], world->bodies[j]);
            }
        }
    }
}

void apply_force(PhysicsBody* body, Vector2 force) {
    if (!body->is_static) {
        Vector2 acceleration = vector2_divide(force, body->mass);
        body->acceleration = vector2_add(body->acceleration, acceleration);
    }
}

bool check_collision(PhysicsBody* a, PhysicsBody* b) {
    return shape_collide(&a->shape, &b->shape);
}

void resolve_collision(PhysicsBody* a, PhysicsBody* b) {
    if (a->is_static && b->is_static) return;

    Vector2 normal = vector2_normalize(vector2_subtract(b->position, a->position));
    Vector2 relative_velocity = vector2_subtract(b->velocity, a->velocity);
    float velocity_along_normal = vector2_dot(relative_velocity, normal);

    if (velocity_along_normal > 0) return;

    float e = MIN(a->restitution, b->restitution);
    float j = -(1 + e) * velocity_along_normal;
    j /= (1 / a->mass) + (1 / b->mass);

    Vector2 impulse = vector2_multiply(normal, j);

    if (!a->is_static) {
        a->velocity = vector2_subtract(a->velocity, vector2_multiply(impulse, 1 / a->mass));
    }

    if (!b->is_static) {
        b->velocity = vector2_add(b->velocity, vector2_multiply(impulse, 1 / b->mass));
    }

    // Separate the objects to prevent sinking
    const float percent = 0.2f; // usually 20% to 80%
    const float slop = 0.01f; // usually 0.01 to 0.1
    Vector2 correction = vector2_multiply(normal, MAX(velocity_along_normal - slop, 0.0f) / ((1 / a->mass) + (1 / b->mass)) * percent);
    
    if (!a->is_static) {
        a->position = vector2_subtract(a->position, vector2_multiply(correction, 1 / a->mass));
        shape_translate(&a->shape, vector2_multiply(correction, -1 / a->mass));
    }
    
    if (!b->is_static) {
        b->position = vector2_add(b->position, vector2_multiply(correction, 1 / b->mass));
        shape_translate(&b->shape, vector2_multiply(correction, 1 / b->mass));
    }
}

void physics_world_cleanup(PhysicsWorld* world) {
    for (int i = 0; i < world->body_count; i++) {
        if (world->bodies[i] != NULL) {
            COG_DELETE(world->bodies[i]);
            world->bodies[i] = NULL;
        }
    }
    world->body_count = 0;
    cog_log_info("Physics world cleaned up");
}