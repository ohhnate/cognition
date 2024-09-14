#ifndef CAMERA_H
#define CAMERA_H

#include "core/utils/cog_types.h"
#include <SDL.h>

typedef struct {
    Vector2 position;
    Vector2 target;
    float zoom;
    float rotation;
    Vector2 offset;
    SDL_Rect viewport;
} Camera;

void camera_init(Camera* camera, int screen_width, int screen_height);
void camera_update(Camera* camera, float delta_time);
void camera_set_position(Camera* camera, Vector2 position);
void camera_set_target(Camera* camera, Vector2 target);
void camera_set_zoom(Camera* camera, float zoom);
void camera_set_rotation(Camera* camera, float rotation);
Vector2 camera_world_to_screen(const Camera* camera, Vector2 world_pos);
Vector2 camera_screen_to_world(const Camera* camera, Vector2 screen_pos);
void camera_apply(const Camera* camera, SDL_Renderer* renderer);
void camera_revert(const Camera* camera, SDL_Renderer* renderer);
SDL_Rect camera_transform_rect(const Camera* camera, SDL_Rect rect);

#endif // CAMERA_H