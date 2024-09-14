#include "cog_camera.h"
#include "core/utils/cog_math.h"
#include <math.h>

void camera_init(Camera* camera, int screen_width, int screen_height) {
    camera->position = (Vector2){0, 0};
    camera->target = (Vector2){0, 0};
    camera->zoom = 1.0f;
    camera->rotation = 0.0f;
    camera->offset = (Vector2){screen_width / 2.0f, screen_height / 2.0f};
    camera->viewport = (SDL_Rect){0, 0, screen_width, screen_height};
}

void camera_update(Camera* camera, float delta_time) {
    // Smoothly move camera towards target
    const float smooth_speed = 5.0f;
    camera->position = vector2_lerp(camera->position, camera->target, smooth_speed * delta_time);
}

void camera_set_position(Camera* camera, Vector2 position) {
    camera->position = position;
}

void camera_set_target(Camera* camera, Vector2 target) {
    camera->target = target;
}

void camera_set_zoom(Camera* camera, float zoom) {
    camera->zoom = CLAMP(zoom, 0.1f, 10.0f);
}

void camera_set_rotation(Camera* camera, float rotation) {
    camera->rotation = rotation;
}

Vector2 camera_world_to_screen(const Camera* camera, Vector2 world_pos) {
    Vector2 view = vector2_subtract(world_pos, camera->position);

    float cos_rot = cosf(-camera->rotation);
    float sin_rot = sinf(-camera->rotation);
    float x_rotated = view.x * cos_rot - view.y * sin_rot;
    float y_rotated = view.x * sin_rot + view.y * cos_rot;
    view = (Vector2){x_rotated, y_rotated};
    
    view = vector2_multiply(view, camera->zoom);
    return vector2_add(view, camera->offset);
}

Vector2 camera_screen_to_world(const Camera* camera, Vector2 screen_pos) {
    Vector2 view = vector2_subtract(screen_pos, camera->offset);
    
    view = vector2_multiply(view, 1.0f / camera->zoom);
    
    float cos_rot = cosf(camera->rotation);
    float sin_rot = sinf(camera->rotation);
    float x_rotated = view.x * cos_rot - view.y * sin_rot;
    float y_rotated = view.x * sin_rot + view.y * cos_rot;
    view = (Vector2){x_rotated, y_rotated};
    
    return vector2_add(view, camera->position);
}

void camera_apply(const Camera* camera, SDL_Renderer* renderer) {
    // Set the clip rectangle to camera viewport
    SDL_RenderSetViewport(renderer, &camera->viewport);
    
    // apply the camera transformations when rendering objects
    // instead of modifying the renderer's state
}

void camera_revert(const Camera* camera, SDL_Renderer* renderer) {
    SDL_RenderSetViewport(renderer, NULL);
}

SDL_Rect camera_transform_rect(const Camera* camera, SDL_Rect rect) {
    Vector2 top_left = camera_world_to_screen(camera, (Vector2){(float)rect.x, (float)rect.y});
    Vector2 bottom_right = camera_world_to_screen(camera, (Vector2){(float)(rect.x + rect.w), (float)(rect.y + rect.h)});
    SDL_Rect transformed = {
        (int)top_left.x,
        (int)top_left.y,
        (int)(bottom_right.x - top_left.x),
        (int)(bottom_right.y - top_left.y)
    };
    return transformed;
}