#include "physics.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include <math.h>
#include <stdlib.h>

void physics_world_init(PhysicsWorld* world) {
    world->body_count = 0;
    world->gravity = (Vector2D){0, 9.8f};
    world->time_step = 1.0f / 60.0f;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        world->bodies[i] = NULL;
    }
    cog_log_info("Physics world initialized");
}

int physics_body_create(PhysicsWorld* world, float x, float y, float width, float height, float mass, bool is_static) {
    if (world->body_count >= MAX_ENTITIES) {
        cog_log_error("Maximum number of physics bodies reached");
        return -1;
    }

    PhysicsBody* body = COG_NEW(PhysicsBody);
    if (body == NULL) {
        cog_log_error("Failed to allocate memory for physics body");
        return -1;
    }

    body->position = (Vector2D){x, y};
    body->velocity = (Vector2D){0, 0};
    body->acceleration = (Vector2D){0, 0};
    body->mass = mass;
    body->collider = (SDL_Rect){(int)x, (int)y, (int)width, (int)height};
    body->is_static = is_static;

    world->bodies[world->body_count] = body;
    cog_log_debug("Physics body created at position (%f, %f)", x, y);
    return world->body_count++;
}

void physics_world_update(PhysicsWorld* world) {
    for (int i = 0; i < world->body_count; i++) {
        PhysicsBody* body = world->bodies[i];
        if (!body->is_static) {
            // Apply gravity
            body->acceleration.y += world->gravity.y;

            // Update velocity
            body->velocity.x += body->acceleration.x * world->time_step;
            body->velocity.y += body->acceleration.y * world->time_step;

            // Update position
            body->position.x += body->velocity.x * world->time_step;
            body->position.y += body->velocity.y * world->time_step;

            // Update collider
            body->collider.x = (int)body->position.x;
            body->collider.y = (int)body->position.y;

            // Reset acceleration
            body->acceleration = (Vector2D){0, 0};
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

void apply_force(PhysicsBody* body, Vector2D force) {
    if (!body->is_static) {
        body->acceleration.x += force.x / body->mass;
        body->acceleration.y += force.y / body->mass;
    }
}

bool check_collision(PhysicsBody* a, PhysicsBody* b) {
    return SDL_HasIntersection(&a->collider, &b->collider);
}

void resolve_collision(PhysicsBody* a, PhysicsBody* b) {
    // Simple collision response
    if (!a->is_static && !b->is_static) {
        // Calculate relative velocity
        Vector2D relative_velocity = {
            b->velocity.x - a->velocity.x,
            b->velocity.y - a->velocity.y
        };

        // Calculate collision normal
        Vector2D normal = {
            b->position.x - a->position.x,
            b->position.y - a->position.y
        };
        float length = (float)sqrt(normal.x * normal.x + normal.y * normal.y);
        normal.x /= length;
        normal.y /= length;

        // Calculate relative velocity along the normal
        float velocity_along_normal = relative_velocity.x * normal.x + relative_velocity.y * normal.y;

        // Do not resolve if velocities are separating
        if (velocity_along_normal > 0) {
            return;
        }

        // Calculate restitution (assuming perfectly elastic collisions)
        float restitution = 1.0f;

        // Calculate impulse scalar
        float impulse_scalar = -(1 + restitution) * velocity_along_normal / (1/a->mass + 1/b->mass);

        // Apply impulse
        Vector2D impulse = {impulse_scalar * normal.x, impulse_scalar * normal.y};
        a->velocity.x -= impulse.x / a->mass;
        a->velocity.y -= impulse.y / a->mass;
        b->velocity.x += impulse.x / b->mass;
        b->velocity.y += impulse.y / b->mass;
    }
}

void physics_world_cleanup(PhysicsWorld* world) {
    if (world == NULL) {
        cog_log_error("Attempted to clean up NULL physics world");
        return;
    }

    for (int i = 0; i < world->body_count; i++) {
        if (world->bodies[i] != NULL) {
            COG_DELETE(world->bodies[i]);
            world->bodies[i] = NULL;
        }
    }
    world->body_count = 0;
    cog_log_info("Physics world cleaned up: %d bodies freed", world->body_count);
}