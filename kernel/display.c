#include "unios/display.h"
#include "stdint.h"
#include "assert.h"
#include "arch/x86.h"
#include "stdio.h"
#include "unios/protect.h"
#include "unios/proc.h"
#include "unios/memory.h"
#include "string.h"
#include "unios/page.h"
#include "unios/tracing.h"

//! We use bga and vbe for display
//! because we are using qemu simulator
//! and it is assumed that bga and vbe are available

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA  0x01CF

#define VBE_DISPI_INDEX_ID          0x0
#define VBE_DISPI_INDEX_XRES        0x1
#define VBE_DISPI_INDEX_YRES        0x2
#define VBE_DISPI_INDEX_BPP         0x3
#define VBE_DISPI_INDEX_ENABLE      0x4
#define VBE_DISPI_INDEX_BANK        0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH  0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 0x7
#define VBE_DISPI_INDEX_X_OFFSET    0x8
#define VBE_DISPI_INDEX_Y_OFFSET    0x9

#define VBE_DISPI_DISABLED    0x00
#define VBE_DISPI_ENABLED     0x01
#define VBE_DISPI_LFB_ENABLED 0x40

static uint16_t bga_screen_width, bga_screen_height;
static uint32_t bga_screen_line_size, bga_screen_buffer_size;
bool     display_available;
uint32_t bga_buf_paddr;
/*!
 * \brief Write to VBE register
 *
 * \attention this function is internal and should not be used outside this file
 *
 * \param cmd Register command/index
 * \param data Data to write to the register
 */
static inline void bga_internal_write_reg(uint16_t cmd, uint16_t data) {
    outw(VBE_DISPI_IOPORT_INDEX, cmd);
    outw(VBE_DISPI_IOPORT_DATA, data);
}

/*!
 * \brief Read from VBE register
 *
 * \attention this function is internal and should not be used outside this file
 *
 * \param cmd Register command/index
 * \return Data read from the register
 */
static inline uint16_t bga_internal_read_reg(uint16_t cmd) {
    outw(VBE_DISPI_IOPORT_INDEX, cmd);
    return inw(VBE_DISPI_IOPORT_DATA);
}

/*!
 * \brief Set resolution and other parameters
 *
 * \attention this function is internal and should not be used outside this file
 *
 * \param width Width
 * \param height Height
 */
static void bga_internal_set_resolution(uint16_t width, uint16_t height) {
    bga_internal_write_reg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    bga_internal_write_reg(VBE_DISPI_INDEX_XRES, width);
    bga_internal_write_reg(VBE_DISPI_INDEX_YRES, height);
    bga_internal_write_reg(VBE_DISPI_INDEX_VIRT_WIDTH, width);
    bga_internal_write_reg(VBE_DISPI_INDEX_VIRT_HEIGHT, (uint16_t)height * 2);
    bga_internal_write_reg(VBE_DISPI_INDEX_BPP, 32);
    bga_internal_write_reg(VBE_DISPI_INDEX_X_OFFSET, 0);
    bga_internal_write_reg(VBE_DISPI_INDEX_Y_OFFSET, 0);
    bga_internal_write_reg(
        VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);
    bga_internal_write_reg(VBE_DISPI_INDEX_BANK, 0);

    bga_screen_line_size = (uint32_t)width * 4;
}

/*!
 * \brief Set resolution and other parameters
 *
 * \param width Width
 * \param height Height
 */
static void bga_set_resolution(uint32_t width, uint32_t height) {
    bga_internal_set_resolution(width, height);
    bga_screen_width       = width;
    bga_screen_height      = height;
    bga_screen_buffer_size = bga_screen_line_size * height * 2;
}

/*!
 * \brief bga ioctl
 *
 * \param cmd command
 * \param arg argument
 *
 * \return 0 if success, -1 if fail
 */
int bga_ioctl(uint32_t cmd, uint32_t arg) {
    uint32_t y_offset = 0;
    switch (cmd) {
        case BGA_GET_HEIGHT:
            return bga_screen_height;
        case BGA_GET_WIDTH:
            return bga_screen_width;
        case BGA_GET_SCALE:
            return 1;
        case BGA_GET_BUFFER:
            if (arg == 0)
                return bga_buf_paddr;
            else
                return bga_buf_paddr + bga_screen_buffer_size / 2;
        case BGA_SWAP_BUFFERS:
            y_offset = bga_screen_height * (arg & 1);
            bga_internal_write_reg(
                VBE_DISPI_INDEX_Y_OFFSET, (uint16_t)y_offset);
            return 0;
        case BGA_DISABLE:
            bga_internal_write_reg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
            return 0;
        case BGA_SET_WIDTH:
            bga_screen_width = arg;
            return 0;
        case BGA_SET_HEIGHT:
            bga_screen_height = arg;
            return 0;
        case BGA_SET_UPDATE:
            bga_set_resolution(bga_screen_width, bga_screen_height);
            return 0;
        default:
            return -1;
    }
}

/*!
 * \brief init display
 *
 * \param dev display device
 *
 * \return 0 if success, -1 if fail
 */
int init_display(pci_dev_t *dev) {
    kinfo("init display\n");
    if (dev->type != DEVICE_DISPLAY) {
        kwarn("display device unavailable\n");
        return -1;
    }
    bga_buf_paddr = pci_read_bar(dev, 0) & 0xfffffff0;
    bga_set_resolution(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    kinfo(
        "bga buf paddr=%p; buf size= %d \n",
        bga_buf_paddr,
        bga_screen_buffer_size);
    uint32_t new_cr3 = rcr3();
    kdebug("new_cr3=%p\n", new_cr3);
    pg_init_video(new_cr3, bga_buf_paddr, bga_screen_buffer_size);
    kdebug("bga mapped in kernel cr3=%p\n", new_cr3);
    return 0;
}
