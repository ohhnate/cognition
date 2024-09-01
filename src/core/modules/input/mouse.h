#ifndef MOUSE_H
#define MOUSE_H

#include <SDL.h>

typedef struct {
    int x, y;
    Uint32 button_state;
    Uint32 previous_button_state;
} Mouse;

void mouse_init(Mouse* mouse);
void mouse_update(Mouse* mouse);
void mouse_handle_event(Mouse* mouse, SDL_Event* event);
int mouse_is_button_pressed(Mouse* mouse, Uint32 button);
int mouse_is_button_released(Mouse* mouse, Uint32 button);
int mouse_is_button_held(Mouse* mouse, Uint32 button);
void mouse_get_position(Mouse* mouse, int* x, int* y);
void mouse_cleanup(Mouse* mouse);

#endif // MOUSE_H