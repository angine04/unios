#include <mouse.h>

mouse_t mouse_status = {
    .buttons = 0,
    .x       = 100,
    .y       = 100,
};

mouse_t get_mouse_status() {
    uint32_t mouse_msg = readmouse();
    // NOTE: mouse message format:
    // 31-28: button, 4 bits
    // 27-14: x, 14 bits
    // 13-0: y, 14 bits
    int y                = mouse_msg & 0x3FFF;
    int x                = (mouse_msg & 0x3FFF0000) >> 14;
    int button           = (mouse_msg & 0xF0000000) >> 28;
    mouse_status.buttons = button;
    mouse_status.x       = x;
    mouse_status.y       = y;
    return mouse_status;
}
