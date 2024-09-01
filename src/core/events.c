#include "events.h"

void event_system_init(EventSystem* system) {
    system->listener_count = 0;
}

int event_system_add_listener(EventSystem* system, EventListener listener, void* user_data) {
    if (system->listener_count >= MAX_LISTENERS) {
        return 0;  // Error: maximum number of listeners reached
    }

    system->listeners[system->listener_count] = listener;
    system->user_data[system->listener_count] = user_data;
    system->listener_count++;

    return 1;  // Success
}

void event_system_remove_listener(EventSystem* system, EventListener listener) {
    for (int i = 0; i < system->listener_count; i++) {
        if (system->listeners[i] == listener) {
            // Remove the listener by shifting all subsequent listeners
            for (int j = i; j < system->listener_count - 1; j++) {
                system->listeners[j] = system->listeners[j + 1];
                system->user_data[j] = system->user_data[j + 1];
            }
            system->listener_count--;
            return;
        }
    }
}

void event_system_dispatch(EventSystem* system, SDL_Event* event) {
    for (int i = 0; i < system->listener_count; i++) {
        system->listeners[i](event, system->user_data[i]);
    }
}

void event_system_shutdown(EventSystem* system) {
    // Perform any necessary cleanup
    system->listener_count = 0;
}