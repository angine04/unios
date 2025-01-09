#pragma once

#include <unios/syscall.h>

#define MOUSE_MAX_EVENTS 10

enum mouse_buttons {
    MOUSE_NONE = 0,
    MOUSE_LEFT_BUTTON   = 1,
    MOUSE_RIGHT_BUTTON  = 2,
    MOUSE_MIDDLE_BUTTON = 4,
};

enum mouse_events {
    MOUSE_NONE_EVENT = 0,
    MOUSE_LEFT_DOWN    = 1,
    MOUSE_RIGHT_DOWN   = 2,
    MOUSE_MIDDLE_DOWN  = 3,
    MOUSE_LEFT_UP      = 4,
    MOUSE_RIGHT_UP     = 5,
    MOUSE_MIDDLE_UP    = 6,
};

typedef struct mouse_event_s {
    int event;
    int x;
    int y;
} mouse_event_t;

typedef struct mouse_s {
    int buttons; //<! button down flags
    int x;   //<! position x
    int y;   //<! position y
} mouse_t;

extern mouse_event_t mouse_events[MOUSE_MAX_EVENTS];
extern int mouse_event_count;
extern int mouse_event_head;
extern int mouse_event_tail;

/*!
 * \brief readmouse syscall
 * \return mouse status
 */
uint32_t readmouse();

/*!
 * \brief get mouse status
 * \return mouse status
 */
mouse_t get_mouse_status();

/*!
 * \brief push mouse event
 * \param event mouse event
 * \param x x position
 * \param y y position
 * \return true if event is pushed, false if event is not pushed
 */
bool push_mouse_event(int event, int x, int y);

/*!
 * \brief pop mouse event
 * \return mouse event
 */
mouse_event_t pop_mouse_event();

/*!
 * \brief check if mouse event queue is empty
 * \return true if mouse event queue is empty, false otherwise
 */
bool mouse_event_empty();

/*!
 * \brief check if mouse event queue is full
 * \return true if mouse event queue is full, false otherwise
 */
bool mouse_event_full();
