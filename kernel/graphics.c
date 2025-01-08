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
    // kdebug("putframe\n");
    // put frame to video memory
    copy_to_kernel(framebuffer, (uint32_t *)buffer, size, pid);

    return 0;
}
