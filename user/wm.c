#include <stdio.h>
#include <stdbool.h>
#include <graphics.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t   screen_vram[1024 * 768];


int main() {
    int pid = get_pid();

    for (size_t i = 0; i < 1024 * 768; i++)
    {
        screen_vram[i] = COLOR_BLUE;
    }

    putframe(screen_vram, 1024 * 768 * 4, pid);

    while (1)
    {
        /* code */
    }
    unreachable();
}
