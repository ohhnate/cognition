#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>
#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"

typedef struct InputManager {
    Keyboard* keyboard;
    Mouse* mouse;
    Gamepad* gamepad;
} InputManager;

void input_manager_init(InputManager* manager);
void input_manager_update(InputManager* manager);
void input_manager_handle_event(InputManager* manager, SDL_Event* event);
void input_manager_cleanup(InputManager* manager);

#endif // INPUT_MANAGER_H