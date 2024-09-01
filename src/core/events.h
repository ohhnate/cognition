#ifndef EVENTS_H
#define EVENTS_H

#include <SDL.h>

#define MAX_LISTENERS 100

typedef void (*EventListener)(SDL_Event* event, void* user_data);

typedef struct EventSystem {
    EventListener listeners[MAX_LISTENERS];
    void* user_data[MAX_LISTENERS];
    int listener_count;
} EventSystem;

// Initialize the event system
void event_system_init(EventSystem* system);

// Add a listener to the event system
int event_system_add_listener(EventSystem* system, EventListener listener, void* user_data);

// Remove a listener from the event system
void event_system_remove_listener(EventSystem* system, EventListener listener);

// Dispatch an event to all listeners
void event_system_dispatch(EventSystem* system, SDL_Event* event);

// Clean up the event system
void event_system_shutdown(EventSystem* system);

#endif // EVENTS_H