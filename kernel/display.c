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
#include "unios/pci.h"

// PCI 配置空间寄存器偏移量
#define PCI_VENDOR_ID 0x00
#define PCI_DEVICE_ID 0x02
#define PCI_COMMAND   0x04

#define PCI_COMMAND_IO     0x1     // Enable response in I/O space
#define PCI_COMMAND_MEMORY 0x2     // Enable response in Memory space
#define PCI_COMMAND_MASTER 0x4     // Enable bus mastering

static inline uint16_t pci_read_word(pci_dev_t* dev, uint8_t offset) {
    return (uint16_t)(pci_read(dev->bus, dev->device, dev->function, offset) & 0xFFFF);
}

static inline void pci_write_word(pci_dev_t* dev, uint8_t offset, uint16_t value) {
    pci_write(dev->bus, dev->device, dev->function, offset, value);
}

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

#define VBE_DISPI_MMIO_BASE 0x500  // bochs dispi interface registers offset in MMIO

typedef struct __attribute__((packed)) {
    volatile uint16_t index;    // VBE寄存器索引
    volatile uint16_t data;     // VBE寄存器数据
} bga_mmio_regs_t;

static volatile uint8_t* bga_mmio_base;
static uint16_t bga_screen_width, bga_screen_height;
static uint32_t bga_screen_line_size, bga_screen_buffer_size;
bool     display_available;
uint32_t bga_buf_paddr;

static inline void bga_internal_write_reg(uint16_t cmd, uint16_t data) {
    // 在 MMIO 中，BGA 寄存器是平坦映射的，使用 (index << 1) 作为偏移
    volatile uint16_t* reg = (volatile uint16_t*)(bga_mmio_base + VBE_DISPI_MMIO_BASE + (cmd << 1));
    *reg = data;
    __asm__ volatile("" ::: "memory");
}

static inline uint16_t bga_internal_read_reg(uint16_t cmd) {
    // 在 MMIO 中，BGA 寄存器是平坦映射的，使用 (index << 1) 作为偏移
    volatile uint16_t* reg = (volatile uint16_t*)(bga_mmio_base + VBE_DISPI_MMIO_BASE + (cmd << 1));
    uint16_t data = *reg;
    __asm__ volatile("" ::: "memory");
    return data;
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
    // 先完全禁用显示
    bga_internal_write_reg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);

    // 等待一下确保状态稳定
    for(int i = 0; i < 1000; i++) {}

    // 设置基本参数
    bga_internal_write_reg(VBE_DISPI_INDEX_XRES, width);
    bga_internal_write_reg(VBE_DISPI_INDEX_YRES, height);
    bga_internal_write_reg(VBE_DISPI_INDEX_BPP, 32);

    // 设置虚拟显示区域
    bga_internal_write_reg(VBE_DISPI_INDEX_VIRT_WIDTH, width);
    bga_internal_write_reg(VBE_DISPI_INDEX_VIRT_HEIGHT, height);  // 先不用双缓冲

    // 重置偏移
    bga_internal_write_reg(VBE_DISPI_INDEX_X_OFFSET, 0);
    bga_internal_write_reg(VBE_DISPI_INDEX_Y_OFFSET, 0);

    // 设置bank
    bga_internal_write_reg(VBE_DISPI_INDEX_BANK, 0);

    // 重新启用显示
    bga_internal_write_reg(VBE_DISPI_INDEX_ENABLE,
                          VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);

    bga_screen_line_size = (uint32_t)width * 4;
    bga_screen_buffer_size = bga_screen_line_size * height;  // 修改buffer大小计算
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

    // 获取并打印PCI配置信息
    uint16_t vendor_id = pci_read_word(dev, PCI_VENDOR_ID);
    uint16_t device_id = pci_read_word(dev, PCI_DEVICE_ID);
    kdebug("Display device: vendor=0x%x, device=0x%x\n", vendor_id, device_id);

    // 验证设备ID
    if (vendor_id != 0x1234 || device_id != 0x1111) {
        kwarn("不支持的显示设备\n");
        return -1;
    }

    // 启用PCI设备的MMIO功能
    uint16_t command = pci_read_word(dev, PCI_COMMAND);
    command |= PCI_COMMAND_MEMORY | PCI_COMMAND_IO;
    pci_write_word(dev, PCI_COMMAND, command);
    kdebug("PCI command register: 0x%x\n", command);

    // 先用I/O端口方式验证设备是否工作
    outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_ID);
    uint16_t io_id = inw(VBE_DISPI_IOPORT_DATA);
    kdebug("BGA ID (I/O port): 0x%x\n", io_id);

    if (io_id == 0xFFFF) {
        kwarn("BGA device not responding (I/O port)\n");
        return -1;
    }

    // 获取帧缓冲区和MMIO地址
    bga_buf_paddr = pci_read_bar(dev, 0) & 0xfffffff0;
    uint32_t mmio_paddr = pci_read_bar(dev, 2) & 0xfffffff0;

    kdebug("Frame buffer: 0x%x, MMIO: 0x%x\n", bga_buf_paddr, mmio_paddr);

    // 映射MMIO区域
    uint32_t cr3 = rcr3();
    bool ok = pg_map_laddr(
        cr3,
        (uint32_t)K_PHY2LIN(mmio_paddr),
        mmio_paddr,
        PG_P | PG_U | PG_RWX,
        PG_P | PG_S | PG_RWX
    );

    if (!ok) {
        kwarn("failed to map BGA MMIO registers\n");
        return -1;
    }

    bga_mmio_base = (volatile uint8_t*)K_PHY2LIN(mmio_paddr);
    kdebug("MMIO virtual address: %p\n", bga_mmio_base);

    // 尝试读取设备ID验证MMIO是否工作
    uint16_t mmio_id = bga_internal_read_reg(VBE_DISPI_INDEX_ID);
    kdebug("BGA ID (MMIO): 0x%x\n", mmio_id);

    if (mmio_id == 0xFFFF) {
        kwarn("BGA device not responding (MMIO)\n");
        // 如果MMIO不工作，回退到I/O端口方式
        kdebug("Falling back to I/O port mode\n");
        return -1;
    }

    // 设置分辨率
    bga_set_resolution(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    // 映射帧缓冲区
    pg_init_video(cr3, bga_buf_paddr, bga_screen_buffer_size);

    return 0;
}
