#pragma once

#include <stdint.h>
#include <stdbool.h>
#define MOUSE_IN_BYTES     3
#define MOUSE_SPEED 1

typedef struct mouse_status_s {
    int buttons; //<! button down flags
    int x;   //<! position x
    int y;   //<! position y
    bool initialized;
} mouse_status_t;

typedef struct mouse_inbuf_s {
    int     count;
    uint8_t buf[MOUSE_IN_BYTES];
} mouse_buf_t;

/*!
 * \brief mouse interrupt handler
 * \param irq irq number
 */
void mouse_handler(int irq);

/*!
 * \brief mouse init
 */
void init_mouse();

/*!
 * \brief read mouse status, internally used by syscall
 * \return mouse status
 */
uint32_t do_readmouse();

/*!
 * \brief read mouse status
 * \return mouse status
 */
uint32_t readmouse();
