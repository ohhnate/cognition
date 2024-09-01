#ifndef EVENTS_H
#define EVENTS_H

#include <SDL.h>

#define MAX_LISTENERS 256

typedef void (*EventListener)(void* event_data, void* user_data);

typedef struct {
    EventListener listeners[MAX_LISTENERS];
    void* user_data[MAX_LISTENERS];
    int listener_count;
} EventSystem;

void event_system_init(EventSystem* system);
int add_listener(EventSystem* system, EventListener listener, void* user_data);
void remove_listener(EventSystem* system, EventListener listener);
void dispatch(EventSystem* system, void* event_data);
void event_system_cleanup(EventSystem* system);

#endif // EVENTS_H