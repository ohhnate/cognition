#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL.h>

#define MAX_KEYS 256

typedef struct {
    Uint8 current_state[MAX_KEYS];
    Uint8 previous_state[MAX_KEYS];
} Keyboard;

void keyboard_init(Keyboard* keyboard);
void keyboard_update(Keyboard* keyboard);
void keyboard_handle_event(Keyboard* keyboard, SDL_Event* event);
int keyboard_is_key_pressed(Keyboard* keyboard, SDL_Scancode key);
int keyboard_is_key_released(Keyboard* keyboard, SDL_Scancode key);
int keyboard_is_key_held(Keyboard* keyboard, SDL_Scancode key);
void keyboard_cleanup(Keyboard* keyboard);

#endif // KEYBOARD_H