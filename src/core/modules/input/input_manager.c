#include "input_manager.h"
#include <stdlib.h>

void input_manager_init(InputManager* manager) {
    manager->keyboard = malloc(sizeof(Keyboard));
    manager->mouse = malloc(sizeof(Mouse));
    manager->gamepad = malloc(sizeof(Gamepad));
    
    keyboard_init(manager->keyboard);
    mouse_init(manager->mouse);
    gamepad_init(manager->gamepad);
}

void input_manager_update(InputManager* manager) {
    keyboard_update(manager->keyboard);
    mouse_update(manager->mouse);
    gamepad_update(manager->gamepad);
}

void input_manager_handle_event(InputManager* manager, SDL_Event* event) {
    keyboard_handle_event(manager->keyboard, event);
    mouse_handle_event(manager->mouse, event);
    gamepad_handle_event(manager->gamepad, event);
}

void input_manager_cleanup(InputManager* manager) {
    keyboard_cleanup(manager->keyboard);
    mouse_cleanup(manager->mouse);
    gamepad_cleanup(manager->gamepad);
    
    free(manager->keyboard);
    free(manager->mouse);
    free(manager->gamepad);
}