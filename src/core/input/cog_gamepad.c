#include "cog_gamepad.h"
#include <string.h>

void gamepad_init(Gamepad* gamepad) {
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    gamepad->gamepad_count = 0;
    
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i) && gamepad->gamepad_count < MAX_GAMEPADS) {
            SDL_GameController* controller = SDL_GameControllerOpen(i);
            if (controller) {
                GamepadState* state = &gamepad->gamepads[gamepad->gamepad_count];
                state->controller = controller;
                state->id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
                memset(state->button_state, 0, sizeof(state->button_state));
                memset(state->previous_button_state, 0, sizeof(state->previous_button_state));
                memset(state->axis_state, 0, sizeof(state->axis_state));
                gamepad->gamepad_count++;
            }
        }
    }
}

void gamepad_update(Gamepad* gamepad) {
    for (int i = 0; i < gamepad->gamepad_count; ++i) {
        GamepadState* state = &gamepad->gamepads[i];
        memcpy(state->previous_button_state, state->button_state, sizeof(state->button_state));
        
        for (int j = 0; j < MAX_GAMEPAD_BUTTONS; ++j) {
            state->button_state[j] = SDL_GameControllerGetButton(state->controller, j);
        }
        
        for (int j = 0; j < MAX_GAMEPAD_AXES; ++j) {
            state->axis_state[j] = SDL_GameControllerGetAxis(state->controller, j);
        }
    }
}

void gamepad_handle_event(Gamepad* gamepad, SDL_Event* event) {
    if (event->type == SDL_CONTROLLERDEVICEADDED) {
        if (gamepad->gamepad_count < MAX_GAMEPADS) {
            int index = event->cdevice.which;
            SDL_GameController* controller = SDL_GameControllerOpen(index);
            if (controller) {
                GamepadState* state = &gamepad->gamepads[gamepad->gamepad_count];
                state->controller = controller;
                state->id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
                memset(state->button_state, 0, sizeof(state->button_state));
                memset(state->previous_button_state, 0, sizeof(state->previous_button_state));
                memset(state->axis_state, 0, sizeof(state->axis_state));
                gamepad->gamepad_count++;
            }
        }
    } else if (event->type == SDL_CONTROLLERDEVICEREMOVED) {
        for (int i = 0; i < gamepad->gamepad_count; ++i) {
            if (gamepad->gamepads[i].id == event->cdevice.which) {
                SDL_GameControllerClose(gamepad->gamepads[i].controller);
                // Shift remaining gamepads
                for (int j = i; j < gamepad->gamepad_count - 1; ++j) {
                    gamepad->gamepads[j] = gamepad->gamepads[j + 1];
                }
                gamepad->gamepad_count--;
                break;
            }
        }
    }
}

int gamepad_is_button_pressed(Gamepad* gamepad, int gamepad_index, SDL_GameControllerButton button) {
    if (gamepad_index < 0 || gamepad_index >= gamepad->gamepad_count) return 0;
    GamepadState* state = &gamepad->gamepads[gamepad_index];
    return state->button_state[button] && !state->previous_button_state[button];
}

int gamepad_is_button_released(Gamepad* gamepad, int gamepad_index, SDL_GameControllerButton button) {
    if (gamepad_index < 0 || gamepad_index >= gamepad->gamepad_count) return 0;
    GamepadState* state = &gamepad->gamepads[gamepad_index];
    return !state->button_state[button] && state->previous_button_state[button];
}

int gamepad_is_button_held(Gamepad* gamepad, int gamepad_index, SDL_GameControllerButton button) {
    if (gamepad_index < 0 || gamepad_index >= gamepad->gamepad_count) return 0;
    GamepadState* state = &gamepad->gamepads[gamepad_index];
    return state->button_state[button];
}

float gamepad_get_axis_value(Gamepad* gamepad, int gamepad_index, SDL_GameControllerAxis axis) {
    if (gamepad_index < 0 || gamepad_index >= gamepad->gamepad_count) return 0.0f;
    GamepadState* state = &gamepad->gamepads[gamepad_index];
    return state->axis_state[axis] / 32767.0f;  // Normalize to [-1, 1]
}

void gamepad_cleanup(Gamepad* gamepad) {
    for (int i = 0; i < gamepad->gamepad_count; ++i) {
        SDL_GameControllerClose(gamepad->gamepads[i].controller);
    }
    gamepad->gamepad_count = 0;
}