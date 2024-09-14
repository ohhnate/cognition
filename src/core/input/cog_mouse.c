#include "cog_mouse.h"

void mouse_init(Mouse* mouse) {
    mouse->x = 0;
    mouse->y = 0;
    mouse->button_state = 0;
    mouse->previous_button_state = 0;
}

void mouse_update(Mouse* mouse) {
    mouse->previous_button_state = mouse->button_state;
    mouse->button_state = SDL_GetMouseState(&mouse->x, &mouse->y);
}

void mouse_handle_event(Mouse* mouse, SDL_Event* event) {
    if (event->type == SDL_MOUSEMOTION) {
        mouse->x = event->motion.x;
        mouse->y = event->motion.y;
    }
}

int mouse_is_button_pressed(Mouse* mouse, Uint32 button) {
    return (mouse->button_state & SDL_BUTTON(button)) && !(mouse->previous_button_state & SDL_BUTTON(button));
}

int mouse_is_button_released(Mouse* mouse, Uint32 button) {
    return !(mouse->button_state & SDL_BUTTON(button)) && (mouse->previous_button_state & SDL_BUTTON(button));
}

int mouse_is_button_held(Mouse* mouse, Uint32 button) {
    return mouse->button_state & SDL_BUTTON(button);
}

void mouse_get_position(Mouse* mouse, int* x, int* y) {
    *x = mouse->x;
    *y = mouse->y;
}

void mouse_cleanup(Mouse* mouse) {
    // No cleanup needed for mouse
}