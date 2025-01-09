#include <mouse.h>

mouse_event_t mouse_events[MOUSE_MAX_EVENTS];
int mouse_event_count = 0;
int mouse_event_head  = 0;
int mouse_event_tail  = 0;

mouse_t mouse_status = {
    .buttons = 0,
    .x       = 100,
    .y       = 100,
};

#define INVALID_MOUSE_MSG 0x80000000

mouse_t get_mouse_status() {
    uint32_t mouse_msg = readmouse();

    if (mouse_msg & INVALID_MOUSE_MSG) {
        return mouse_status;
    }

    // NOTE: mouse message format:
    // 31: valid when 0, 1 bit
    // 30-28: button, 3 bits
    // 27-14: x, 14 bits
    // 13-0: y, 14 bits
    int y                = mouse_msg & 0x3FFF;
    int x                = (mouse_msg >> 14) & 0x3FFF;
    int button           = (mouse_msg >> 28) & 0x7;

    if (button & MOUSE_LEFT_BUTTON && !(mouse_status.buttons & MOUSE_LEFT_BUTTON)) {
        push_mouse_event(MOUSE_LEFT_DOWN, x, y);
    }
    if (button & MOUSE_RIGHT_BUTTON && !(mouse_status.buttons & MOUSE_RIGHT_BUTTON)) {
        push_mouse_event(MOUSE_RIGHT_DOWN, x, y);
    }
    if (button & MOUSE_MIDDLE_BUTTON && !(mouse_status.buttons & MOUSE_MIDDLE_BUTTON)) {
        push_mouse_event(MOUSE_MIDDLE_DOWN, x, y);
    }
    if (!(button & MOUSE_LEFT_BUTTON) && (mouse_status.buttons & MOUSE_LEFT_BUTTON)) {
        push_mouse_event(MOUSE_LEFT_UP, x, y);
    }
    if (!(button & MOUSE_RIGHT_BUTTON) && (mouse_status.buttons & MOUSE_RIGHT_BUTTON)) {
        push_mouse_event(MOUSE_RIGHT_UP, x, y);
    }
    if (!(button & MOUSE_MIDDLE_BUTTON) && (mouse_status.buttons & MOUSE_MIDDLE_BUTTON)) {
        push_mouse_event(MOUSE_MIDDLE_UP, x, y);
    }

    mouse_status.buttons = button;
    mouse_status.x       = x;
    mouse_status.y       = y;
    return mouse_status;
}

bool push_mouse_event(int event, int x, int y) {
    if (mouse_event_full()) {
        return false;
    }
    mouse_events[mouse_event_tail].event = event;
    mouse_events[mouse_event_tail].x     = x;
    mouse_events[mouse_event_tail].y     = y;
    mouse_event_tail = (mouse_event_tail + 1) % MOUSE_MAX_EVENTS;
    mouse_event_count++;
    return true;
}

mouse_event_t pop_mouse_event() {
    if (mouse_event_empty()) {
        return (mouse_event_t){MOUSE_NONE_EVENT, 0, 0};
    }
    mouse_event_t event = mouse_events[mouse_event_head];
    mouse_event_head = (mouse_event_head + 1) % MOUSE_MAX_EVENTS;
    mouse_event_count--;
    return event;
}

bool mouse_event_empty() {
    return mouse_event_count == 0;
}

bool mouse_event_full() {
    return mouse_event_count == MOUSE_MAX_EVENTS;
}
