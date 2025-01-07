#pragma once
#include <unios/pci.h>
#include <lib/stdbool.h>
#include <lib/stdint.h>

#define BGA_SWAP_BUFFERS 0x0101
#define BGA_GET_HEIGHT   0x0102
#define BGA_GET_WIDTH    0x0103
#define BGA_GET_SCALE    0x0104
#define BGA_DISABLE      0x0105
#define BGA_SET_WIDTH    0x0106
#define BGA_SET_HEIGHT   0x0107
#define BGA_SET_UPDATE   0x0108
#define BGA_GET_BUFFER   0x0109

#define DISPLAY_WIDTH  1024
#define DISPLAY_HEIGHT 768

// ！ We are using only bga as a display device
// ！ because we are using qemu simulator
// ！ and it is assumed that bga is available

bool     display_available;
uint32_t bga_buf_paddr;

/*!
 * \brief init bga
 *
 * \param dev bga device
 *
 * \return 0 if success, -1 if fail
 */
int init_display(pci_dev_t* dev);

/*!
 * \brief bga ioctl
 *
 * \param cmd command
 * \param arg argument
 *
 * \return 0 if success, -1 if fail
 */
int bga_ioctl(uint32_t cmd, uint32_t arg);
