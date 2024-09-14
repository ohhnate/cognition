#include "cog_events.h"

void event_system_init(EventSystem* system) {
    system->listener_count = 0;
}

int add_listener(EventSystem* system, EventListener listener, void* user_data) {
    if (system->listener_count >= MAX_LISTENERS) {
        return 0;
    }
    system->listeners[system->listener_count] = listener;
    system->user_data[system->listener_count] = user_data;
    system->listener_count++;
    return 1;
}

void remove_listener(EventSystem* system, EventListener listener) {
    for (int i = 0; i < system->listener_count; i++) {
        if (system->listeners[i] == listener) {
            for (int j = i; j < system->listener_count - 1; j++) {
                system->listeners[j] = system->listeners[j + 1];
                system->user_data[j] = system->user_data[j + 1];
            }
            system->listener_count--;
            return;
        }
    }
}

void dispatch(EventSystem* system, void* event_data) {
    for (int i = 0; i < system->listener_count; i++) {
        system->listeners[i](event_data, system->user_data[i]);
    }
}

void event_system_cleanup(EventSystem* system) {
    system->listener_count = 0;
}