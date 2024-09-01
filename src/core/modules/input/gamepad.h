#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <SDL.h>

#define MAX_GAMEPADS 4
#define MAX_GAMEPAD_BUTTONS 15
#define MAX_GAMEPAD_AXES 6

typedef struct {
    SDL_GameController* controller;
    SDL_JoystickID id;
    Uint8 button_state[MAX_GAMEPAD_BUTTONS];
    Uint8 previous_button_state[MAX_GAMEPAD_BUTTONS];
    Sint16 axis_state[MAX_GAMEPAD_AXES];
} GamepadState;

typedef struct {
    GamepadState gamepads[MAX_GAMEPADS];
    int gamepad_count;
} Gamepad;

void gamepad_init(Gamepad* gamepad);
void gamepad_update(Gamepad* gamepad);
void gamepad_handle_event(Gamepad* gamepad, SDL_Event* event);
int gamepad_is_button_pressed(Gamepad* gamepad, int gamepad_index, SDL_GameControllerButton button);
int gamepad_is_button_released(Gamepad* gamepad, int gamepad_index, SDL_GameControllerButton button);
int gamepad_is_button_held(Gamepad* gamepad, int gamepad_index, SDL_GameControllerButton button);
float gamepad_get_axis_value(Gamepad* gamepad, int gamepad_index, SDL_GameControllerAxis axis);
void gamepad_cleanup(Gamepad* gamepad);

#endif // GAMEPAD_H