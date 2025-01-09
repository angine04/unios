#include <unios/mouse.h>
#include <unios/interrupt.h>
#include <unios/keymap.h>
#include <arch/x86.h>
#include <sys/defs.h>
#include <unios/tracing.h>
#include <unios/syscall.h>
#include <math.h>

static mouse_status_t mouse_status;
static mouse_buf_t    mouse_in;

void mouse_handler(int irq) {
    uint8_t scan_code = inb(0x60);
    if (!mouse_status.initialized) {
        mouse_status.initialized = true;
        return;
    }

    mouse_in.buf[mouse_in.count++] = scan_code;

    disable_int_begin();

    if (mouse_in.count != 3) {
        enable_int();
        return;
    }

    uint8_t flags         = mouse_in.buf[0];
    bool    left_button   = flags & 0b1;
    bool    right_button  = flags & 0b10;
    bool    middle_button = flags & 0b100;
    bool    sync          = flags & 0b1000;
    bool    x_sign        = flags & 0b10000;
    bool    y_sign        = flags & 0b100000;
    bool    x_overflow    = flags & 0b1000000;
    bool    y_overflow    = flags & 0b10000000;
    int8_t  x_move        = mouse_in.buf[1];
    int8_t  y_move        = mouse_in.buf[2];

    //kdebug("mouse_in buf: %8b %d %d\n", flags, x_move, y_move);

    //! check sync bit
    if (!sync) {
        mouse_in.count = 0;
        enable_int();
        kdebug("mouse sync bit not set, ignore\n");
        return;
    }

    if (x_overflow || y_overflow) {
        mouse_in.count = 0;
        enable_int();
        kdebug("mouse overflow, ignore\n");
        return;
    }

    if (left_button) {
        mouse_status.buttons |= MOUSE_LEFT_BUTTON;
    } else {
        mouse_status.buttons &= ~MOUSE_LEFT_BUTTON;
    }

    if (right_button) {
        mouse_status.buttons |= MOUSE_RIGHT_BUTTON;
    } else {
        mouse_status.buttons &= ~MOUSE_RIGHT_BUTTON;
    }

    if (middle_button) {
        mouse_status.buttons |= MOUSE_MIDDLE_BUTTON;
    } else {
        mouse_status.buttons &= ~MOUSE_MIDDLE_BUTTON;
    }

    mouse_status.x += x_move * MOUSE_SPEED;
    mouse_status.y -= y_move * MOUSE_SPEED;

    mouse_status.x = clamp(mouse_status.x, 0, DISPLAY_WIDTH);
    mouse_status.y = clamp(mouse_status.y, 0, DISPLAY_HEIGHT);

    // kdebug("mouse_in.x: %d, mouse_in.y: %d\n", mouse_status.x, mouse_status.y);
    // kdebug("mouse_in.buttons: %d\n", mouse_status.buttons);

    // must comment all kdebug out
    // otherwise it may stuck mysteriously
    // and the mouse will not work
    // I have no idea why but it works

    mouse_in.count = 0;
    disable_int_end();
}

void init_mouse() {
    mouse_in.count = 0;
    mouse_status.x = 100;
    mouse_status.y = 100;
    mouse_status.buttons = 0;

    put_irq_handler(MOUSE_IRQ, mouse_handler);
    enable_irq(MOUSE_IRQ);
}

uint32_t do_readmouse() {
    uint32_t x = mouse_status.x & 0x3FFF;
    uint32_t y = mouse_status.y & 0x3FFF;
    uint32_t buttons = mouse_status.buttons & 0b0111;
    mouse_status.buttons = 0;

    //NOTE: mouse message format:
    // 31: valid when 0, 1 bit
    // 30-28: button, 3 bits
    // 27-14: x, 14 bits
    // 13-0: y, 14 bits

    int ret = (buttons << 28) | (x << 14) | y;
    kdebug("readmouse: %32b\n", ret);
    return ret;
}
