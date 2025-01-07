#include <unios/syscall.h>
#include <stdint.h>
#include <arch/x86.h>
#include <unios/tracing.h>
#include <assert.h>
#include <unios/protect.h>
#include <unios/proc.h>
#include <unios/page.h>
#include <unios/pci.h>
#include <unios/display.h>
#include <unios/schedule.h>
#include <string.h>
#include "graphics.h"
#include "stdint.h"

#define PAGE_SIZE 4096

int do_putframe(void *buffer, size_t size, int pid) {
    if (buffer == NULL) {
        kwarn("buffer is null while putframe");
        return -1;
    }

    // get framebuffer address
    uint32_t *framebuffer = (uint32_t *)bga_ioctl(BGA_GET_BUFFER, 0);
    // kdebug("framebuffer: %p\n", framebuffer);
    // kdebug("buffer: %p\n", buffer);
    // put frame to video memory
    copy_to_kernel(framebuffer, (uint32_t *)buffer, size, pid);

    return 0;
}

int copy_to_kernel(uint32_t *dst, uint32_t *src, int size, int pid) {
    uint32_t kernel_lin_addr, current_user_lin_addr, current_user_phy_addr,
        offset;

    disable_int();
    // load cr3
    lcr3(0x100000);

    // calculate the offset of virtual address and physical address
    kernel_lin_addr       = (uint32_t)dst;
    current_user_lin_addr = (uint32_t)src;

    current_user_phy_addr =
        pg_laddr_phyaddr(proc_table[pid]->pcb.cr3, current_user_lin_addr);
    offset                 = current_user_lin_addr & 0xfff;
    current_user_phy_addr += offset;

    if (size < PAGE_SIZE) {
        memcpy(
            (void *)kernel_lin_addr,
            (void *)K_PHY2LIN(current_user_phy_addr),
            size);
        lcr3(proc_table[pid]->pcb.cr3);
        enable_int();
        return 0;
    }

    memcpy(
        (void *)kernel_lin_addr,
        (void *)K_PHY2LIN(current_user_phy_addr),
        (PAGE_SIZE - offset));

    size                  -= (PAGE_SIZE - offset);
    kernel_lin_addr       -= offset;
    current_user_lin_addr -= offset;

    while (size >= PAGE_SIZE) {
        kernel_lin_addr       += PAGE_SIZE;
        current_user_lin_addr += PAGE_SIZE;
        current_user_phy_addr =
            pg_laddr_phyaddr(proc_table[pid]->pcb.cr3, current_user_lin_addr);
        size -= (PAGE_SIZE);
        memcpy(
            (void *)kernel_lin_addr,
            (void *)K_PHY2LIN(current_user_phy_addr),
            (PAGE_SIZE));
    }
    if (size > 0) {
        kernel_lin_addr       += PAGE_SIZE;
        current_user_lin_addr += PAGE_SIZE;
        current_user_phy_addr =
            pg_laddr_phyaddr(proc_table[pid]->pcb.cr3, current_user_lin_addr);
        memcpy(
            (void *)kernel_lin_addr,
            (void *)K_PHY2LIN(current_user_phy_addr),
            size);
    }

    lcr3(proc_table[pid]->pcb.cr3);

    enable_int();
    return 0;
}
