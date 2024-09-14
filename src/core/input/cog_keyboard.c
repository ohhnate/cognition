#include "cog_keyboard.h"
#include <string.h>

void keyboard_init(Keyboard* keyboard) {
    memset(keyboard->current_state, 0, sizeof(keyboard->current_state));
    memset(keyboard->previous_state, 0, sizeof(keyboard->previous_state));
}

void keyboard_update(Keyboard* keyboard) {
    memcpy(keyboard->previous_state, keyboard->current_state, sizeof(keyboard->current_state));
    const Uint8* state = SDL_GetKeyboardState(NULL);
    memcpy(keyboard->current_state, state, MAX_KEYS);
}

void keyboard_handle_event(Keyboard* keyboard, SDL_Event* event) {
    // This function can be used for specific key events if needed
}

int keyboard_is_key_pressed(Keyboard* keyboard, SDL_Scancode key) {
    return keyboard->current_state[key] && !keyboard->previous_state[key];
}

int keyboard_is_key_released(Keyboard* keyboard, SDL_Scancode key) {
    return !keyboard->current_state[key] && keyboard->previous_state[key];
}

int keyboard_is_key_held(Keyboard* keyboard, SDL_Scancode key) {
    return keyboard->current_state[key];
}

void keyboard_cleanup(Keyboard* keyboard) {
    // No cleanup needed for keyboard
}