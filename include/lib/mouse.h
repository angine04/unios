#pragma once

#include <unios/syscall.h>

typedef struct mouse_s {
    int buttons; //<! button down flags
    int x;   //<! position x
    int y;   //<! position y
} mouse_t;

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
